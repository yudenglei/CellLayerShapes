#ifndef FLUXCAE_COMMAND_H
#define FLUXCAE_COMMAND_H

#include <memory>
#include <string>
#include <vector>
#include <functional>
#include <stack>
#include <deque>

namespace fluxcae {

// 前向声明
class Layout;

/// @brief 命令接口
/// @details 所有可撤销操作都实现此接口
class ICommand {
public:
    virtual ~ICommand() = default;
    
    /// @brief 执行命令
    virtual void execute() = 0;
    
    /// @brief 撤销命令
    virtual void undo() = 0;
    
    /// @brief 命令描述
    virtual std::string description() const = 0;
    
    /// @brief 是否可以合并（用于批量操作优化）
    virtual bool canMergeWith(const ICommand* other) const { return false; }
    
    /// @brief 合并命令（可选实现）
    virtual void mergeWith(const ICommand* other) {}
    
    /// @brief 是否是空操作
    virtual bool isNoOp() const { return false; }
};

/// @brief 命令栈管理器
/// @details 管理Undo/Redo历史
class CommandStack {
private:
    std::deque<std::unique_ptr<ICommand>> undo_stack_;
    std::deque<std::unique_ptr<ICommand>> redo_stack_;
    
    // 配置
    size_t max_history_size_ = 100;
    
    // 当前是否在执行中（防止递归）
    bool is_executing_ = false;
    
public:
    CommandStack() = default;
    ~CommandStack() = default;
    
    // 删除拷贝
    CommandStack(const CommandStack&) = delete;
    CommandStack& operator=(const CommandStack&) = delete;
    
    // 移动语义
    CommandStack(CommandStack&&) noexcept = default;
    CommandStack& operator=(CommandStack&&) noexcept = default;
    
    /// @brief 执行命令并加入历史
    void execute(std::unique_ptr<ICommand> cmd);
    
    /// @brief 撤销
    void undo();
    
    /// @brief 重做
    void redo();
    
    /// @brief 是否可以撤销
    bool canUndo() const { return !undo_stack_.empty(); }
    
    /// @brief 是否可以重做
    bool canRedo() const { return !redo_stack_.empty(); }
    
    /// @brief 获取撤销描述
    std::string undoDescription() const;
    
    /// @brief 获取重做描述
    std::string redoDescription() const;
    
    /// @brief 清空历史
    void clear();
    
    /// @brief 设置最大历史记录数
    void setMaxHistorySize(size_t size);
    
    /// @brief 当前历史大小
    size_t historySize() const { return undo_stack_.size(); }
    
    /// @brief 当前重做栈大小
    size_t redoSize() const { return redo_stack_.size(); }
    
    // ===== 信号（用于UI更新） =====
    std::function<void()> onCommandExecuted;
    std::function<void()> onCommandUndone;
    std::function<void()> onCommandRedone;
    std::function<void()> onStackCleared;
    
private:
    void notifyExecuted();
    void notifyUndone();
    void notifyRedone();
    void notifyCleared();
    void trimHistory();
};

/// @brief 事务组
/// @details 将多个命令打包为一个原子操作
class Transaction {
private:
    CommandStack& stack_;
    std::vector<std::unique_ptr<ICommand>> commands_;
    bool committed_ = false;
    bool canceled_ = false;
    std::string name_;
    
public:
    Transaction(CommandStack& stack, const std::string& name = "Transaction");
    ~Transaction();
    
    // 删除拷贝
    Transaction(const Transaction&) = delete;
    Transaction& operator=(const Transaction&) = delete;
    
    /// @brief 添加命令到事务
    void addCommand(std::unique_ptr<ICommand> cmd);
    
    /// @brief 提交事务（执行所有命令）
    void commit();
    
    /// @brief 回滚事务（取消所有命令）
    void cancel();
    
    /// @brief 获取命令数量
    size_t size() const { return commands_.size(); }
    
    /// @brief 是否已提交
    bool isCommitted() const { return committed_; }
    
    /// @brief 是否已取消
    bool isCanceled() const { return canceled_; }
};

/// @brief 便捷事务创建函数
inline Transaction beginTransaction(CommandStack& stack, const std::string& name = "Transaction") {
    return Transaction(stack, name);
}

// ===== 常用命令实现 =====

/// @brief 空操作命令
class NoOpCommand : public ICommand {
public:
    void execute() override {}
    void undo() override {}
    std::string description() const override { return "No Operation"; }
    bool isNoOp() const override { return true; }
};

/// @brief 组合命令
class MacroCommand : public ICommand {
private:
    std::vector<std::unique_ptr<ICommand>> commands_;
    std::string description_;
    
public:
    MacroCommand(std::string desc = "Macro") : description_(std::move(desc)) {}
    
    void addCommand(std::unique_ptr<ICommand> cmd) {
        commands_.push_back(std::move(cmd));
    }
    
    void execute() override {
        for (auto& cmd : commands_) {
            cmd->execute();
        }
    }
    
    void undo() override {
        for (auto it = commands_.rbegin(); it != commands_.rend(); ++it) {
            (*it)->undo();
        }
    }
    
    std::string description() const override { return description_; }
};

} // namespace fluxcae

#endif // FLUXCAE_COMMAND_H

/**
 * @file Command.cpp
 * @brief Command pattern implementation for undo/redo
 * @author FluxCAE
 * @date 2024
 */
#include "fluxcae/service/transaction/Command.h"
#include "fluxcae/foundation/utils/Logger.h"

namespace fluxcae {
namespace service {
namespace transaction {

// Command implementation
Command::~Command() = default;

void Command::execute() {
    if (!isExecuted_) {
        doExecute();
        isExecuted_ = true;
    }
}

void Command::undo() {
    if (isExecuted_) {
        doUndo();
        isExecuted_ = false;
    }
}

bool Command::isExecuted() const {
    return isExecuted_;
}

void Command::setLabel(const std::string& label) {
    label_ = label;
}

const std::string& Command::label() const {
    return label_;
}

// MacroCommand implementation
MacroCommand::MacroCommand() = default;

MacroCommand::~MacroCommand() {
    for (auto cmd : commands_) {
        delete cmd;
    }
}

void MacroCommand::add(Command* cmd) {
    if (cmd) {
        commands_.push_back(cmd);
    }
}

void MacroCommand::doExecute() {
    for (auto cmd : commands_) {
        cmd->execute();
    }
}

void MacroCommand::doUndo() {
    // Undo in reverse order
    for (auto it = commands_.rbegin(); it != commands_.rend(); ++it) {
        (*it)->undo();
    }
}

size_t MacroCommand::size() const {
    return commands_.size();
}

// CommandStack implementation
CommandStack::CommandStack(size_t maxSize)
    : maxSize_(maxSize), currentIndex_(0) {
}

CommandStack::~CommandStack() = default;

void CommandStack::execute(Command* cmd) {
    if (!cmd) return;
    
    // 如果在撤销过程中执行新命令，删除撤销部分
    if (currentIndex_ < commands_.size()) {
        for (size_t i = currentIndex_; i < commands_.size(); ++i) {
            delete commands_[i];
        }
        commands_.erase(commands_.begin() + currentIndex_, commands_.end());
    }
    
    cmd->execute();
    commands_.push_back(cmd);
    currentIndex_ = commands_.size();
    
    // 如果超过最大大小，删除最旧的命令
    if (commands_.size() > maxSize_) {
        delete commands_.front();
        commands_.erase(commands_.begin());
        currentIndex_--;
    }
    
    LOG_DEBUG("Command executed: {}", cmd->label());
}

bool CommandStack::canUndo() const {
    return currentIndex_ > 0;
}

bool CommandStack::canRedo() const {
    return currentIndex_ < commands_.size();
}

void CommandStack::undo() {
    if (!canUndo()) return;
    
    currentIndex_--;
    commands_[currentIndex_]->undo();
    
    LOG_DEBUG("Command undone: {}", commands_[currentIndex_]->label());
}

void CommandStack::redo() {
    if (!canRedo()) return;
    
    commands_[currentIndex_]->execute();
    currentIndex_++;
    
    LOG_DEBUG("Command redone: {}", commands_[currentIndex_ - 1]->label());
}

void CommandStack::clear() {
    for (auto cmd : commands_) {
        delete cmd;
    }
    commands_.clear();
    currentIndex_ = 0;
}

size_t CommandStack::size() const {
    return commands_.size();
}

size_t CommandStack::undoCount() const {
    return currentIndex_;
}

size_t CommandStack::redoCount() const {
    return commands_.size() - currentIndex_;
}

// Transaction implementation
Transaction::Transaction(const std::string& name)
    : name_(name), committed_(false), isActive_(true) {
}

Transaction::~Transaction() {
    if (!committed_ && isActive_) {
        rollback();
    }
    
    for (auto cmd : commands_) {
        delete cmd;
    }
}

void Transaction::addCommand(Command* cmd) {
    if (cmd && isActive_) {
        commands_.push_back(cmd);
    }
}

void Transaction::commit() {
    if (!isActive_) return;
    
    // 执行所有未执行的命令
    for (auto cmd : commands_) {
        if (!cmd->isExecuted()) {
            cmd->execute();
        }
    }
    
    committed_ = true;
    isActive_ = false;
    LOG_DEBUG("Transaction committed: {}", name_);
}

void Transaction::rollback() {
    if (!isActive_) return;
    
    // 逆向撤销所有命令
    for (auto it = commands_.rbegin(); it != commands_.rend(); ++it) {
        if ((*it)->isExecuted()) {
            (*it)->undo();
        }
    }
    
    isActive_ = false;
    LOG_DEBUG("Transaction rolled back: {}", name_);
}

bool Transaction::isCommitted() const {
    return committed_;
}

size_t Transaction::commandCount() const {
    return commands_.size();
}

} // namespace transaction
} // namespace service
} // namespace fluxcae

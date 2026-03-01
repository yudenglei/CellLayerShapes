#ifndef FLUXCAE_PLUGIN_H
#define FLUXCAE_PLUGIN_H

#include <string>
#include <memory>
#include <vector>
#include <map>
#include <functional>

namespace fluxcae {

// 前向声明
class Layout;
class Layer;
class IPlugin;

/// @brief 插件接口基类
class IPlugin {
public:
    virtual ~IPlugin() = default;
    
    /// @brief 插件名称
    virtual std::string name() const = 0;
    
    /// @brief 插件版本
    virtual std::string version() const = 0;
    
    /// @brief 插件描述
    virtual std::string description() const = 0;
    
    /// @brief 初始化
    virtual bool initialize() = 0;
    
    /// @brief 关闭
    virtual void shutdown() = 0;
    
    /// @brief 插件类型
    enum class Type {
        Tool,           // 交互工具
        Importer,       // 文件导入
        Exporter,       // 文件导出
        DRC,             // 设计规则检查
        ERC,             // 电气规则检查
        Simulation,     // 仿真
        NetlistImport,  // 网表导入
        NetlistExport,  // 网表导出
        Other           // 其他
    };
    
    virtual Type type() const = 0;
    
    /// @brief 是否已初始化
    bool isInitialized() const { return initialized_; }
    
protected:
    bool initialized_ = false;
};

/// @brief 工具插件接口
class IToolPlugin : public IPlugin {
public:
    Type type() const override { return Type::Tool; }
    
    /// @brief 创建工具实例
    virtual class ITool* createTool() = 0;
    
    /// @brief 工具图标路径
    virtual std::string iconPath() const { return ""; }
    
    /// @brief 工具快捷键
    virtual std::string shortcut() const { return ""; }
    
    /// @brief 工具类别
    enum class ToolCategory {
        Select,     // 选择
        Draw,       // 绘制
        Measure,    // 测量
        View,       // 视图
        Other       // 其他
    };
    
    virtual ToolCategory category() const { return ToolCategory::Other; }
};

/// @brief 导入器插件接口
class IImporterPlugin : public IPlugin {
public:
    Type type() const override { return Type::Importer; }
    
    /// @brief 支持的文件扩展名
    virtual std::vector<std::string> supportedExtensions() const = 0;
    
    /// @brief 导入文件
    virtual bool importFile(const std::string& path, Layout& layout) = 0;
    
    /// @brief 文件过滤器描述
    virtual std::string fileFilter() const = 0;
};

/// @brief 导出器插件接口
class IExporterPlugin : public IPlugin {
public:
    Type type() const override { return Type::Exporter; }
    
    /// @brief 支持的文件扩展名
    virtual std::vector<std::string> supportedExtensions() const = 0;
    
    /// @brief 导出文件
    virtual bool exportFile(const std::string& path, const Layout& layout) = 0;
    
    /// @brief 文件过滤器描述
    virtual std::string fileFilter() const = 0;
};

/// @brief DRC规则插件接口
class IDRCPlugin : public IPlugin {
public:
    Type type() const override { return Type::DRC; }
    
    /// @brief DRC规则
    struct DRCRule {
        std::string name;
        std::string description;
        std::string category;
        bool enabled = true;
    };
    
    /// @brief DRC违规
    struct DRCViolation {
        std::string rule_name;
        std::string message;
        int layer_id;
        int x, y;
        int severity; // 1=error, 2=warning, 3=info
    };
    
    /// @brief 获取规则列表
    virtual std::vector<DRCRule> getRules() const = 0;
    
    /// @brief 执行检查
    virtual std::vector<DRCViolation> check(const Layout& layout) = 0;
};

/// @brief 插件管理器
class PluginManager {
private:
    std::vector<std::unique_ptr<IPlugin>> plugins_;
    std::map<std::string, IPlugin*> name_index_;
    std::map<IPlugin::Type, std::vector<IPlugin*>> type_index_;
    
    std::string plugin_directory_;
    bool auto_discover_ = true;
    
public:
    PluginManager();
    ~PluginManager();
    
    /// @brief 加载插件
    bool loadPlugin(const std::string& path);
    
    /// @brief 卸载插件
    void unloadPlugin(const std::string& name);
    
    /// @brief 卸载所有插件
    void unloadAll();
    
    /// @brief 获取所有插件
    const std::vector<std::unique_ptr<IPlugin>>& plugins() const { return plugins_; }
    
    /// @brief 按类型获取插件
    template<typename T>
    std::vector<T*> getPlugins() const {
        std::vector<T*> result;
        for (auto& plugin : plugins_) {
            if (auto casted = dynamic_cast<T*>(plugin.get())) {
                result.push_back(casted);
            }
        }
        return result;
    }
    
    /// @brief 按名称查找插件
    IPlugin* findPlugin(const std::string& name) const;
    
    /// @brief 初始化所有插件
    bool initializeAll();
    
    /// @brief 关闭所有插件
    void shutdownAll();
    
    /// @brief 设置插件目录
    void setPluginDirectory(const std::string& dir);
    
    /// @brief 获取插件目录
    const std::string& pluginDirectory() const { return plugin_directory_; }
    
    /// @brief 自动发现插件
    void discoverPlugins();
    
    /// @brief 获取可用的导入器
    std::vector<IImporterPlugin*> getImporters() const;
    
    /// @brief 获取可用的导出器
    std::vector<IExporterPlugin*> getExporters() const;
    
    /// @brief 获取可用的DRC规则
    std::vector<IDRCPlugin*> getDRCPlugins() const;
    
    // ===== 信号 =====
    std::function<void(IPlugin*)> onPluginLoaded;
    std::function<void(IPlugin*)> onPluginUnloaded;
    std::function<void(const std::string&)> onPluginLoadError;
};

// 插件导出宏
#if defined(FLUXCAE_PLUGIN_EXPORTS)
#   define FLUXCAE_PLUGIN_API __declspec(dllexport)
#elif defined(FLUXCAE_STATIC_PLUGINS)
#   define FLUXCAE_PLUGIN_API
#else
#   define FLUXCAE_PLUGIN_API __declspec(dllimport)
#endif

/// @brief 插件工厂函数类型
using PluginFactory = IPlugin*(*)();

/// @brief 注册插件的宏
#define REGISTER_PLUGIN(PluginClass) \
    extern "C" FLUXCAE_PLUGIN_API fluxcae::IPlugin* createPlugin() { \
        return new PluginClass(); \
    }

} // namespace fluxcae

#endif // FLUXCAE_PLUGIN_H

/**
 * @file PluginManager.h
 * @brief PluginManager - 插件管理器
 * @author FluxCAE
 * @date 2024
 */
#pragma once

#include "fluxcae/service/plugin/IPlugin.h"
#include <string>
#include <vector>
#include <memory>
#include <unordered_map>
#include <functional>

namespace fluxcae {
namespace service {

/**
 * @brief PluginManager 负责插件的加载、卸载和管理
 */
class PluginManager {
public:
    using PluginPtr = std::shared_ptr<plugin::IPlugin>;
    using PluginList = std::vector<PluginPtr>;

    /**
     * @brief 单例获取
     * @return PluginManager实例
     */
    static PluginManager& instance();

    /**
     * @brief 析构函数
     */
    ~PluginManager();

    // ===== 插件加载 =====

    /**
     * @brief 从文件加载插件
     * @param path 插件路径
     * @return 加载成功返回true
     */
    bool loadPlugin(const std::string& path);

    /**
     * @brief 卸载插件
     * @param pluginName 插件名称
     * @return 卸载成功返回true
     */
    bool unloadPlugin(const std::string& pluginName);

    /**
     * @brief 从目录批量加载插件
     * @param dirPath 插件目录路径
     * @return 加载的插件数量
     */
    size_t loadPluginsFromDirectory(const std::string& dirPath);

    // ===== 插件查询 =====

    /// 获取所有已加载的插件
    const PluginList& plugins() const { return plugins_; }

    /**
     * @brief 根据名称查找插件
     * @param name 插件名称
     * @return 插件指针，未找到返回nullptr
     */
    PluginPtr findPlugin(const std::string& name) const;

    /**
     * @brief 根据类型查找插件
     * @param type 插件类型
     * @return 插件列表
     */
    PluginList findPluginsByType(plugin::PluginType type) const;

    /// 获取已加载插件数量
    size_t pluginCount() const { return plugins_.size(); }

    // ===== 插件目录 =====

    /// 获取插件搜索目录列表
    const std::vector<std::string>& pluginPaths() const { return pluginPaths_; }

    /**
     * @brief 添加插件搜索目录
     * @param path 目录路径
     */
    void addPluginPath(const std::string& path);

    /**
     * @brief 移除插件搜索目录
     * @param path 目录路径
     */
    void removePluginPath(const std::string& path);

    // ===== 生命周期 =====

    /**
     * @brief 初始化所有已加载的插件
     */
    void initializeAll();

    /**
     * @brief 关闭所有已加载的插件
     */
    void shutdownAll();

    /**
     * @brief 卸载所有插件
     */
    void unloadAll();

    // ===== 注册回调 =====

    /**
     * @brief 注册插件加载回调
     * @param callback 回调函数
     */
    void registerLoadCallback(std::function<void(PluginPtr)> callback);

    /**
     * @brief 注册插件卸载回调
     * @param callback 回调函数
     */
    void registerUnloadCallback(std::function<void(PluginPtr)> callback);

private:
    PluginManager() = default;
    PluginManager(const PluginManager&) = delete;
    PluginManager& operator=(const PluginManager&) = delete;

    struct PluginHandle;
    std::unordered_map<std::string, std::unique_ptr<PluginHandle>> loadedPlugins_;
    PluginList plugins_;
    std::vector<std::string> pluginPaths_;
    
    std::vector<std::function<void(PluginPtr)>> loadCallbacks_;
    std::vector<std::function<void(PluginPtr)>> unloadCallbacks_;
};

} // namespace service
} // namespace fluxcae

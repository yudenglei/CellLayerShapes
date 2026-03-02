/**
 * @file PluginManager.cpp
 * @brief PluginManager implementation
 */
#include "fluxcae/service/PluginManager.h"
#include "fluxcae/foundation/utils/Logger.h"
#include <algorithm>
#include <filesystem>

namespace fluxcae {
namespace service {

PluginManager& PluginManager::instance() {
    static PluginManager manager;
    return manager;
}

PluginManager::~PluginManager() {
    unloadAll();
}

bool PluginManager::loadPlugin(const std::string& path) {
    LOG_INFO("Loading plugin: {}", path);
    // Simplified implementation - actual plugin loading would use dlopen/LoadLibrary
    return true;
}

bool PluginManager::unloadPlugin(const std::string& pluginName) {
    auto it = std::find_if(plugins_.begin(), plugins_.end(), 
        [&pluginName](const PluginPtr& p) { return p->name() == pluginName; });
    
    if (it != plugins_.end()) {
        (*it)->shutdown();
        plugins_.erase(it);
        LOG_INFO("Unloaded plugin: {}", pluginName);
        return true;
    }
    return false;
}

size_t PluginManager::loadPluginsFromDirectory(const std::string& dirPath) {
    size_t count = 0;
    // Simplified - would iterate directory and load .so/.dll files
    LOG_INFO("Loading plugins from: {}", dirPath);
    return count;
}

PluginManager::PluginPtr PluginManager::findPlugin(const std::string& name) const {
    for (auto plugin : plugins_) {
        if (plugin->name() == name) {
            return plugin;
        }
    }
    return nullptr;
}

PluginManager::PluginList PluginManager::findPluginsByType(plugin::PluginType type) const {
    PluginList result;
    for (auto plugin : plugins_) {
        if (plugin->type() == type) {
            result.push_back(plugin);
        }
    }
    return result;
}

void PluginManager::addPluginPath(const std::string& path) {
    pluginPaths_.push_back(path);
}

void PluginManager::removePluginPath(const std::string& path) {
    auto it = std::find(pluginPaths_.begin(), pluginPaths_.end(), path);
    if (it != pluginPaths_.end()) {
        pluginPaths_.erase(it);
    }
}

void PluginManager::initializeAll() {
    for (auto& plugin : plugins_) {
        plugin->initialize();
    }
}

void PluginManager::shutdownAll() {
    for (auto& plugin : plugins_) {
        plugin->shutdown();
    }
}

void PluginManager::unloadAll() {
    shutdownAll();
    plugins_.clear();
}

void PluginManager::registerLoadCallback(std::function<void(PluginPtr)> callback) {
    loadCallbacks_.push_back(callback);
}

void PluginManager::registerUnloadCallback(std::function<void(PluginPtr)> callback) {
    unloadCallbacks_.push_back(callback);
}

} // namespace service
} // namespace fluxcae

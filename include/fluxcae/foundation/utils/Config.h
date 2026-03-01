/**
 * @file Config.h
 * @brief Config - 配置管理
 * @author FluxCAE
 * @date 2024
 */
#pragma once

#include <string>
#include <map>
#include <memory>
#include <variant>
#include <vector>

namespace fluxcae {
namespace foundation {

/**
 * @brief Config 配置管理类
 * 
 * 支持：
 * - 配置文件加载（JSON/INI）
 * - 类型安全的值获取
 * - 默认值设置
 * - 配置变更监听
 */
class Config {
public:
    using Value = std::variant<
        std::string,
        int64_t,
        int,
        double,
        bool,
        std::vector<std::string>
    >;

    /**
     * @brief 构造函数
     */
    Config();

    /**
     * @brief 析构函数
     */
    ~Config();

    // ===== 加载/保存 =====

    /**
     * @brief 从JSON文件加载配置
     * @param filePath 文件路径
     * @return 成功返回true
     */
    bool loadFromJson(const std::string& filePath);

    /**
     * @brief 从INI文件加载配置
     * @param filePath 文件路径
     * @return 成功返回true
     */
    bool loadFromIni(const std::string& filePath);

    /**
     * @brief 保存为JSON文件
     * @param filePath 文件路径
     * @return 成功返回true
     */
    bool saveToJson(const std::string& filePath) const;

    /**
     * @brief 保存为INI文件
     * @param filePath 文件路径
     * @return 成功返回true
     */
    bool saveToIni(const std::string& filePath) const;

    // ===== 值获取 =====

    /**
     * @brief 获取字符串值
     * @param key 键
     * @param defaultValue 默认值
     * @return 值或默认值
     */
    std::string getString(const std::string& key, const std::string& defaultValue = "") const;

    /**
     * @brief 获取整数值
     * @param key 键
     * @param defaultValue 默认值
     * @return 值或默认值
     */
    int64_t getInt(const std::string& key, int64_t defaultValue = 0) const;

    /**
     * @brief 获取双精度浮点值
     * @param key 键
     * @param defaultValue 默认值
     * @return 值或默认值
     */
    double getDouble(const std::string& key, double defaultValue = 0.0) const;

    /**
     * @brief 获取布尔值
     * @param key 键
     * @param defaultValue 默认值
     * @return 值或默认值
     */
    bool getBool(const std::string& key, bool defaultValue = false) const;

    /**
     * @brief 获取字符串数组
     * @param key 键
     * @return 字符串向量
     */
    std::vector<std::string> getStringList(const std::string& key) const;

    // ===== 值设置 =====

    /**
     * @brief 设置值
     * @param key 键
     * @param value 值
     */
    void set(const std::string& key, const Value& value);

    /**
     * @brief 设置字符串值
     */
    void setString(const std::string& key, const std::string& value);

    /**
     * @brief 设置整数
     */
    void setInt(const std::string& key, int64_t value);

    /**
     * @brief 设置浮点数
     */
    void setDouble(const std::string& key, double value);

    /**
     * @brief 设置布尔值
     */
    void setBool(const std::string& key, bool value);

    // ===== 查询 =====

    /**
     * @brief 检查键是否存在
     * @param key 键
     * @return 存在返回true
     */
    bool has(const std::string& key) const;

    /**
     * @brief 移除键
     * @param key 键
     * @return 成功返回true
     */
    bool remove(const std::string& key);

    /// 获取所有键
    std::vector<std::string> keys() const;

    /// 清空所有配置
    void clear();

    // ===== 全局配置 =====

    /**
     * @brief 获取全局配置实例
     * @return 配置引用
     */
    static Config& global();

    /**
     * @brief 加载全局配置文件
     * @param filePath 文件路径
     * @return 成功返回true
     */
    static bool loadGlobal(const std::string& filePath);

private:
    std::map<std::string, Value> values_;
};

// 便捷类型定义
using ConfigPtr = std::shared_ptr<Config>;

} // namespace foundation
} // namespace fluxcae

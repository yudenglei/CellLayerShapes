/**
 * @file Netlist.h
 * @brief Netlist - 网络列表管理
 * @author FluxCAE
 * @date 2024
 */
#pragma once

#include "fluxcae/domain/layout/Net.h"
#include "fluxcae/domain/layout/Component.h"
#include "fluxcae/domain/layout/Pin.h"
#include <string>
#include <vector>
#include <unordered_map>
#include <unordered_set>
#include <memory>

namespace fluxcae {
namespace domain {
namespace layout {

/**
 * @brief Netlist 网络列表管理器
 * 
 * 负责管理：
 * - 网络（Net）管理
 * - 器件（Component）管理
 * - 网络与器件引脚的连接关系
 */
class Netlist {
public:
    using Id = uint64_t;
    using NetPtr = std::shared_ptr<Net>;
    using ComponentPtr = std::shared_ptr<Component>;
    using PinPtr = std::shared_ptr<Pin>;
    using NetList = std::vector<NetPtr>;
    using ComponentList = std::vector<ComponentPtr>;

    /**
     * @brief 构造函数
     * @param id 网络列表唯一标识符
     */
    explicit Netlist(Id id = 0);

    /**
     * @brief 析构函数
     */
    ~Netlist() = default;

    // ===== 属性访问 =====

    /// 获取Netlist ID
    Id id() const { return id_; }
    
    /// 设置Netlist ID
    void setId(Id id) { id_ = id; }

    /// 获取网络数量
    size_t netCount() const { return nets_.size(); }
    
    /// 获取器件数量
    size_t componentCount() const { return components_.size(); }

    // ===== 网络管理 =====

    /**
     * @brief 创建新网络
     * @param name 网络名称
     * @return 创建的网络指针
     */
    NetPtr createNet(const std::string& name);

    /**
     * @brief 删除网络
     * @param net 网络指针
     * @return 删除成功返回true
     */
    bool deleteNet(NetPtr net);

    /**
     * @brief 根据名称查找网络
     * @param name 网络名称
     * @return 找到返回网络指针，否则返回nullptr
     */
    NetPtr findNet(const std::string& name) const;

    /// 获取所有网络
    const NetList& nets() const { return nets_; }

    // ===== 器件管理 =====

    /**
     * @brief 创建新器件
     * @param name 器件名称
     * @param type 器件类型
     * @return 创建的器件指针
     */
    ComponentPtr createComponent(const std::string& name, 
                                  ComponentType type = ComponentType::Generic);

    /**
     * @brief 删除器件
     * @param component 器件指针
     * @return 删除成功返回true
     */
    bool deleteComponent(ComponentPtr component);

    /**
     * @brief 根据名称查找器件
     * @param name 器件名称
     * @return 找到返回器件指针，否则返回nullptr
     */
    ComponentPtr findComponent(const std::string& name) const;

    /// 获取所有器件
    const ComponentList& components() const { return components_; }

    // ===== 连接关系管理 =====

    /**
     * @brief 连接网络和引脚
     * @param net 网络
     * @param pin 引脚
     * @return 连接成功返回true
     */
    bool connect(NetPtr net, PinPtr pin);

    /**
     * @brief 断开网络和引脚的连接
     * @param net 网络
     * @param pin 引脚
     * @return 断开成功返回true
     */
    bool disconnect(NetPtr net, PinPtr pin);

    /**
     * @brief 获取引脚所属的网络
     * @param pin 引脚
     * @return 网络指针，如果未连接返回nullptr
     */
    NetPtr getNetForPin(PinPtr pin) const;

    /**
     * @brief 获取网络连接的所有引脚
     * @param net 网络
     * @return 引脚列表
     */
    std::vector<PinPtr> getPinsForNet(NetPtr net) const;

    /**
     * @brief 获取器件的所有引脚
     * @param component 器件
     * @return 引脚列表
     */
    std::vector<PinPtr> getPinsForComponent(ComponentPtr component) const;

    // ===== 拓扑分析 =====

    /**
     * @brief 获取所有未连接（开路）的引脚
     * @return 未连接的引脚列表
     */
    std::vector<PinPtr> getOpenPins() const;

    /**
     * @brief 获取所有短路的网络
     * @return 短路的网络对列表
     */
    std::vector<std::pair<NetPtr, NetPtr>> getShortedNets() const;

    // ===== 清理 =====

    /**
     * @brief 清除所有网络和器件
     */
    void clear();

    /**
     * @brief 清理未连接的网络
     */
    void cleanOrphanNets();

    // ===== 序列化 =====

    /**
     * @brief 导出为字符串表示
     * @return 字符串表示
     */
    std::string toString() const;

private:
    Id id_;
    NetList nets_;
    ComponentList components_;
    
    // 名称索引
    std::unordered_map<std::string, NetPtr> netNameIndex_;
    std::unordered_map<std::string, ComponentPtr> componentNameIndex_;
    
    // 引脚到网络的映射
    std::unordered_map<PinPtr, NetPtr> pinToNetMap_;
    
    // 网络到引脚的映射
    std::unordered_map<NetPtr, std::vector<PinPtr>> netToPinsMap_;
};

} // namespace layout
} // namespace domain
} // namespace fluxcae

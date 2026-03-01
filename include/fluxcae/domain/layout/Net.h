/**
 * @file Net.h
 * @brief Net - 网络，连接多个引脚的电气网络
 * @author FluxCAE
 * @date 2024
 */
#pragma once

#include "fluxcae/domain/layout/Pin.h"
#include "fluxcae/domain/layout/Via.h"
#include <string>
#include <vector>
#include <memory>
#include <unordered_set>

namespace fluxcae {
namespace domain {
namespace layout {

/**
 * @brief NetType 网络类型
 */
enum class NetType {
    Signal,     ///< 信号网络
    Power,      ///< 电源网络（VCC/VDD）
    Ground,     ///< 地网络（GND/VSS）
    Analog,     ///< 模拟信号
    Digital,    ///< 数字信号
    Differential, ///< 差分对
    NoConnect   ///< 无连接（NC）
};

/**
 * @brief Net 表示电气网络
 * 
 * 网络连接多个引脚（Pin）和通孔（Via），形成电气连通性
 */
class Net {
public:
    using Id = uint64_t;
    using PinPtr = std::shared_ptr<Pin>;
    using ViaPtr = std::shared_ptr<Via>;
    using PinList = std::vector<PinPtr>;
    using ViaList = std::vector<ViaPtr>;

    /**
     * @brief 构造函数
     * @param name 网络名称
     * @param id 网络唯一标识符
     */
    Net(const std::string& name, Id id = 0);

    /**
     * @brief 析构函数
     */
    ~Net() = default;

    // ===== 属性访问 =====

    /// 获取网络ID
    Id id() const { return id_; }
    
    /// 设置网络ID
    void setId(Id id) { id_ = id; }

    /// 获取网络名称
    const std::string& name() const { return name_; }
    
    /// 设置网络名称
    void setName(const std::string& name) { name_ = name; }

    /// 获取网络类型
    NetType type() const { return type_; }
    
    /// 设置网络类型
    void setType(NetType type) { type_ = type; }

    // ===== 引脚管理 =====

    /**
     * @brief 添加引脚到网络
     * @param pin 引脚指针
     * @return 添加成功返回true
     */
    bool addPin(PinPtr pin);

    /**
     * @brief 从网络移除引脚
     * @param pin 引脚指针
     * @return 移除成功返回true
     */
    bool removePin(PinPtr pin);

    /// 获取所有引脚
    const PinList& pins() const { return pins_; }
    
    /// 获取引脚数量
    size_t pinCount() const { return pins_.size(); }

    // ===== 通孔管理 =====

    /**
     * @brief 添加通孔到网络
     * @param via 通孔指针
     * @return 添加成功返回true
     */
    bool addVia(ViaPtr via);

    /**
     * @brief 从网络移除通孔
     * @param via 通孔指针
     * @return 移除成功返回true
     */
    bool removeVia(ViaPtr via);

    /// 获取所有通孔
    const ViaList& vias() const { return vias_; }
    
    /// 获取通孔数量
    size_t viaCount() const { return vias_.size(); }

    // ===== 电气属性 =====

    /// 获取网络优先级
    int priority() const { return priority_; }
    
    /// 设置网络优先级
    void setPriority(int p) { priority_ = p; }

    /// 获取电阻（欧姆）
    double resistance() const { return resistance_; }
    
    /// 设置电阻
    void setResistance(double r) { resistance_ = r; }

    /// 获取电容（法拉）
    double capacitance() const { return capacitance_; }
    
    /// 设置电容
    void setCapacitance(double c) { capacitance_ = c; }

    // ===== 状态查询 =====

    /// 判断是否为空网络（无引脚连接）
    bool isEmpty() const { return pins_.empty() && vias_.empty(); }

    /// 判断是否连接到电源
    bool isPower() const { return type_ == NetType::Power; }

    /// 判断是否连接到地
    bool isGround() const { return type_ == NetType::Ground; }

    // ===== 序列化 =====

    /// 获取网络类型字符串
    static const char* typeToString(NetType type);
    
    /// 从字符串获取网络类型
    static NetType stringToType(const std::string& str);

    /**
     * @brief 导出为字符串表示
     * @return 字符串表示
     */
    std::string toString() const;

private:
    Id id_;
    std::string name_;
    NetType type_;
    
    PinList pins_;
    ViaList vias_;
    
    // 电气属性
    int priority_;
    double resistance_;
    double capacitance_;
    
    // 引脚集合用于快速查找
    std::unordered_set<PinPtr> pinSet_;
    std::unordered_set<ViaPtr> viaSet_;
};

} // namespace layout
} // namespace domain
} // namespace fluxcae

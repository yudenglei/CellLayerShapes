/**
 * @file Pin.h
 * @brief Pin - 器件引脚，网络连接的端点
 * @author FluxCAE
 * @date 2024
 */
#pragma once

#include "fluxcae/foundation/math/Point.h"
#include "fluxcae/foundation/math/Box.h"
#include <string>
#include <memory>

namespace fluxcae {
namespace domain {
namespace layout {

// 前向声明
class Net;
class Component;

/**
 * @brief PinDirection 引脚方向
 */
enum class PinDirection {
    Input,      ///< 输入
    Output,     ///< 输出
    Bidirectional, ///< 双向
    Power,      ///< 电源
    Ground,     ///< 地
    NoConnect   ///< 无连接
};

/**
 * @brief PinType 引脚类型
 */
enum class PinType {
    Signal,     ///< 信号
    Analog,    ///< 模拟
    Digital,    ///< 数字
    Clock,      ///< 时钟
    Power,      ///< 电源
    Ground,     ///< 地
    Reset,      ///< 复位
    Test        ///< 测试
};

/**
 * @brief Pin 表示器件引脚
 * 
 * 引脚是电气连接的端点，连接器件和网络
 */
class Pin {
public:
    using Id = uint64_t;
    using NetPtr = std::shared_ptr<Net>;
    using ComponentPtr = std::shared_ptr<Component>;

    /**
     * @brief 构造函数
     * @param name 引脚名称
     * @param position 引脚位置（相对于器件原点）
     * @param id 引脚唯一标识符
     */
    Pin(const std::string& name, const math::Point& position, Id id = 0);

    /**
     * @brief 析构函数
     */
    ~Pin() = default;

    // ===== 属性访问 =====

    /// 获取引脚ID
    Id id() const { return id_; }
    
    /// 设置引脚ID
    void setId(Id id) { id_ = id; }

    /// 获取引脚名称
    const std::string& name() const { return name_; }
    
    /// 设置引脚名称
    void setName(const std::string& name) { name_ = name; }

    /// 获取引脚位置
    const math::Point& position() const { return position_; }
    
    /// 设置引脚位置
    void setPosition(const math::Point& pos) { position_ = pos; }

    /// 获取引脚方向
    PinDirection direction() const { return direction_; }
    
    /// 设置引脚方向
    void setDirection(PinDirection dir) { direction_ = dir; }

    /// 获取引脚类型
    PinType type() const { return type_; }
    
    /// 设置引脚类型
    void setType(PinType t) { type_ = t; }

    // ===== 所属关系 =====

    /// 获取所属器件
    ComponentPtr component() const { return component_; }
    
    /// 设置所属器件
    void setComponent(ComponentPtr comp) { component_ = comp; }

    /// 获取连接的网格（如果已连接）
    NetPtr net() const { return net_; }
    
    /// 设置连接的网络
    void setNet(NetPtr n) { net_ = n; }

    /// 判断是否已连接网络
    bool isConnected() const { return net_ != nullptr; }

    // ===== 几何计算 =====

    /// 获取引脚边界框
    math::Box bbox() const;

    /**
     * @brief 获取引脚在绝对坐标系中的位置
     * @return 绝对坐标位置
     */
    math::Point absolutePosition() const;

    // ===== 序列化 =====

    /// 获取引脚方向字符串
    static const char* directionToString(PinDirection dir);
    
    /// 从字符串获取引脚方向
    static PinDirection stringToDirection(const std::string& str);

    /// 获取引脚类型字符串
    static const char* typeToString(PinType type);
    
    /// 从字符串获取引脚类型
    static PinType stringToType(const std::string& str);

    /**
     * @brief 导出为字符串表示
     * @return 字符串表示
     */
    std::string toString() const;

private:
    Id id_;
    std::string name_;
    math::Point position_;
    PinDirection direction_;
    PinType type_;
    
    ComponentPtr component_;
    NetPtr net_;
};

} // namespace layout
} // namespace domain
} // namespace fluxcae

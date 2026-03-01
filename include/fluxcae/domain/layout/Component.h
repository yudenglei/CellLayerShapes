/**
 * @file Component.h
 * @brief Component - 电子器件
 * @author FluxCAE
 * @date 2024
 */
#pragma once

#include "fluxcae/domain/layout/Pin.h"
#include "fluxcae/domain/layout/Shape.h"
#include "fluxcae/foundation/math/Transform.h"
#include "fluxcae/foundation/math/Box.h"
#include <string>
#include <vector>
#include <memory>
#include <unordered_map>

namespace fluxcae {
namespace domain {
namespace layout {

/**
 * @brief ComponentType 器件类型
 */
enum class ComponentType {
    Generic,        ///< 通用器件
    IC,             ///< 集成电路
    Resistor,       ///< 电阻
    Capacitor,      ///< 电容
    Inductor,       ///< 电感
    Diode,          ///< 二极管
    Transistor,     ///< 晶体管
    Connector,      ///< 连接器
    Crystal,        ///< 晶振
    LED,            ///< 发光二极管
    Switch,         ///< 开关
    Fuse,           ///< 保险丝
    Relay,          ///< 继电器
    Transformer,    ///< 变压器
    Sensor,         ///< 传感器
    Motor,          ///< 马达
    Battery,        ///< 电池
    CrystalOscillator, ///< 晶振模块
    Module          ///< 模块
};

/**
 * @brief Component 表示电子器件
 * 
 * 器件包含：
 * - 引脚（Pin）列表
 * - 封装形状（Package shape）
 * - 电气参数
 */
class Component {
public:
    using Id = uint64_t;
    using PinPtr = std::shared_ptr<Pin>;
    using ShapePtr = std::shared_ptr<Shape>;
    using PinList = std::vector<PinPtr>;

    /**
     * @brief 构造函数
     * @param name 器件名称（如 R1, U1 等）
     * @param type 器件类型
     * @param id 器件唯一标识符
     */
    Component(const std::string& name, ComponentType type = ComponentType::Generic, 
              Id id = 0);

    /**
     * @brief 析构函数
     */
    ~Component() = default;

    // ===== 属性访问 =====

    /// 获取器件ID
    Id id() const { return id_; }
    
    /// 设置器件ID
    void setId(Id id) { id_ = id; }

    /// 获取器件名称
    const std::string& name() const { return name_; }
    
    /// 设置器件名称
    void setName(const std::string& name) { name_ = name; }

    /// 获取器件类型
    ComponentType type() const { return type_; }
    
    /// 设置器件类型
    void setType(ComponentType type) { type_ = type; }

    /// 获取器件封装名称
    const std::string& package() const { return package_; }
    
    /// 设置器件封装名称
    void setPackage(const std::string& pkg) { package_ = pkg; }

    // ===== 变换 =====

    /// 获取变换矩阵
    const math::Transform& transform() const { return transform_; }
    
    /// 获取变换矩阵的可变引用
    math::Transform& transform() { return transform_; }

    /// 获取器件边界框
    math::Box bbox() const;

    // ===== 引脚管理 =====

    /**
     * @brief 创建并添加引脚
     * @param name 引脚名称
     * @param position 引脚位置（相对于器件原点）
     * @return 创建的引脚指针
     */
    PinPtr createPin(const std::string& name, const math::Point& position);

    /**
     * @brief 添加引脚
     * @param pin 引脚指针
     * @return 添加成功返回true
     */
    bool addPin(PinPtr pin);

    /**
     * @brief 移除引脚
     * @param pin 引脚指针
     * @return 移除成功返回true
     */
    bool removePin(PinPtr pin);

    /// 获取所有引脚
    const PinList& pins() const { return pins_; }
    
    /// 获取引脚数量
    size_t pinCount() const { return pins_.size(); }

    /**
     * @brief 根据名称查找引脚
     * @param name 引脚名称
     * @return 找到返回引脚指针，否则返回nullptr
     */
    PinPtr findPin(const std::string& name) const;

    // ===== 封装形状 =====

    /**
     * @brief 添加封装形状
     * @param shape 形状指针
     */
    void addPackageShape(ShapePtr shape);

    /// 获取所有封装形状
    const std::vector<ShapePtr>& packageShapes() const { return packageShapes_; }

    // ===== 器件参数 =====

    /// 获取器件值（如电阻值、电容值）
    const std::string& value() const { return value_; }
    
    /// 设置器件值
    void setValue(const std::string& val) { value_ = val; }

    /// 获取器件描述
    const std::string& description() const { return description_; }
    
    /// 设置器件描述
    void setDescription(const std::string& desc) { description_ = desc; }

    /// 获取器件制造商
    const std::string& manufacturer() const { return manufacturer_; }
    
    /// 设置器件制造商
    void setManufacturer(const std::string& mfr) { manufacturer_ = mfr; }

    /// 获取器件型号
    const std::string& partNumber() const { return partNumber_; }
    
    /// 设置器件型号
    void setPartNumber(const std::string& pn) { partNumber_ = pn; }

    // ===== 序列化 =====

    /// 获取器件类型字符串
    static const char* typeToString(ComponentType type);
    
    /// 从字符串获取器件类型
    static ComponentType stringToType(const std::string& str);

    /**
     * @brief 导出为字符串表示
     * @return 字符串表示
     */
    std::string toString() const;

private:
    Id id_;
    std::string name_;
    ComponentType type_;
    std::string package_;
    math::Transform transform_;
    
    PinList pins_;
    std::unordered_map<std::string, PinPtr> pinNameIndex_;
    
    std::vector<ShapePtr> packageShapes_;
    
    // 器件参数
    std::string value_;
    std::string description_;
    std::string manufacturer_;
    std::string partNumber_;
};

} // namespace layout
} // namespace domain
} // namespace fluxcae

/**
 * @file StackupLayer.h
 * @brief StackupLayer - PCB层叠中的单层
 * @author FluxCAE
 * @date 2024
 */
#pragma once

#include "fluxcae/domain/layout/Material.h"
#include <string>
#include <memory>

namespace fluxcae {
namespace domain {
namespace layout {

/**
 * @brief LayerType 层类型
 */
enum class LayerType {
    Signal,         ///< 信号层
    Plane,          ///< 平面层（电源/地）
    Mixed,          ///< 混合层（信号+平面）
    SolderMaskTop,  ///< 顶层阻焊
    SolderMaskBottom, ///< 底层阻焊
    SilkscreenTop,  ///< 顶层丝印
    SilkscreenBottom, ///< 底层丝印
    PasteTop,       ///< 顶层锡膏
    PasteBottom,    ///< 底层锡膏
    AssemblyTop,    ///< 顶层装配
    AssemblyBottom, ///< 底层装配
    Drill,          ///< 钻孔层
    KeepOut,        ///< 禁止布线层
    Mechanical,     ///< 机械层
    User,           ///< 用户定义层
    Core,           ///< 芯板
    Prepreg,        ///< 半固化片
    Copper          ///< 铜箔
};

/**
 * @brief LayerDirection 层方向
 */
enum class LayerDirection {
    Horizontal,  ///< 水平走线
    Vertical,   ///< 垂直走线
    Any         ///< 任意方向
};

/**
 * @brief StackupLayer 表示PCB层叠中的单层
 */
class StackupLayer {
public:
    using Id = uint64_t;
    using MaterialPtr = std::shared_ptr<Material>;

    /**
     * @brief 构造函数
     * @param name 层名称
     * @param type 层类型
     * @param id 层唯一标识符
     */
    StackupLayer(const std::string& name, LayerType type, Id id = 0);

    /**
     * @brief 析构函数
     */
    ~StackupLayer() = default;

    // ===== 属性访问 =====

    /// 获取层ID
    Id id() const { return id_; }
    
    /// 设置层ID
    void setId(Id i) { id_ = i; }

    /// 获取层名称
    const std::string& name() const { return name_; }
    
    /// 设置层名称
    void setName(const std::string& n) { name_ = n; }

    /// 获取层类型
    LayerType type() const { return type_; }
    
    /// 设置层类型
    void setType(LayerType t) { type_ = t; }

    /// 获取层索引（从0开始）
    int index() const { return index_; }
    
    /// 设置层索引
    void setIndex(int i) { index_ = i; }

    /// 是否为外层
    bool isOuter() const { return isOuter_; }
    
    /// 设置是否为外层
    void setOuter(bool outer) { isOuter_ = outer; }

    /// 是否为电气层
    bool isElectrical() const {
        return type_ == LayerType::Signal || 
               type_ == LayerType::Plane || 
               type_ == LayerType::Mixed;
    }

    // ===== 材料属性 =====

    /// 获取材料
    MaterialPtr material() const { return material_; }
    
    /// 设置材料
    void setMaterial(MaterialPtr m) { material_ = m; }

    /// 获取层厚度（微米）
    double thickness() const;
    
    /// 设置层厚度
    void setThickness(double t);

    /// 获取铜厚（微米）
    double copperThickness() const { return copperThickness_; }
    
    /// 设置铜厚
    void setCopperThickness(double t);

    // ===== 走线属性 =====

    /// 获取走线方向
    LayerDirection direction() const { return direction_; }
    
    /// 设置走线方向
    void setDirection(LayerDirection d) { direction_ = d; }

    /// 获取默认线宽（微米）
    double defaultWidth() const { return defaultWidth_; }
    
    /// 设置默认线宽
    void setDefaultWidth(double w) { defaultWidth_ = w; }

    /// 获取默认间距（微米）
    double defaultSpacing() const { return defaultSpacing_; }
    
    /// 设置默认间距
    void setDefaultSpacing(double s) { defaultSpacing_ = s; }

    /// 获取阻抗目标值（欧姆，可选）
    double impedanceTarget() const { return impedanceTarget_; }
    
    /// 设置阻抗目标值
    void setImpedanceTarget(double z) { impedanceTarget_ = z; }

    // ===== 颜色显示 =====

    /// 获取显示颜色（RGB）
    uint32_t color() const { return color_; }
    
    /// 设置显示颜色
    void setColor(uint32_t c) { color_ = c; }

    /// 获取显示透明度 (0-255)
    uint8_t opacity() const { return opacity_; }
    
    /// 设置显示透明度
    void setOpacity(uint8_t o) { opacity_ = o; }

    /// 是否可见
    bool visible() const { return visible_; }
    
    /// 设置可见性
    void setVisible(bool v) { visible_ = v; }

    /// 是否锁定
    bool locked() const { return locked_; }
    
    /// 设置锁定状态
    void setLocked(bool l) { locked_ = l; }

    // ===== 序列化 =====

    /// 获取层类型字符串
    static const char* typeToString(LayerType type);
    
    /// 从字符串获取层类型
    static LayerType stringToType(const std::string& str);

    /// 获取层方向字符串
    static const char* directionToString(LayerDirection dir);
    
    /// 从字符串获取层方向
    static LayerDirection stringToDirection(const std::string& str);

    /**
    导出为字符串表示 * @brief 
     * @return 字符串表示
     */
    std::string toString() const;

private:
    Id id_;
    std::string name_;
    LayerType type_;
    int index_;
    bool isOuter_;
    
    MaterialPtr material_;
    double thickness_;       // 层厚度（微米）
    double copperThickness_; // 铜厚（微米）
    
    // 走线属性
    LayerDirection direction_;
    double defaultWidth_;
    double defaultSpacing_;
    double impedanceTarget_;
    
    // 显示属性
    uint32_t color_;
    uint8_t opacity_;
    bool visible_;
    bool locked_;
};

// 便捷类型定义
using StackupLayerPtr = std::shared_ptr<StackupLayer>;

} // namespace layout
} // namespace domain
} // namespace fluxcae

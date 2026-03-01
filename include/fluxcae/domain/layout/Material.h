/**
 * @file Material.h
 * @brief Material - 材料属性
 * @author FluxCAE
 * @date 2024
 */
#pragma once

#include <string>
#include <memory>

namespace fluxcae {
namespace domain {
namespace layout {

/**
 * @brief MaterialType 材料类型
 */
enum class MaterialType {
    Dielectric,     ///< 介质材料（PCB基材）
    Conductor,      ///< 导体材料（铜、铝等）
    SolderMask,     ///< 阻焊材料
    Silkscreen,     ///< 丝印材料
    Plating,        ///< 镀层材料
    Adhesive,       ///< 粘合剂
    Core,           ///< 芯板
    Prepreg,        ///< 半固化片
    Coverlay,       ///< 覆盖膜
    Solder,         ///< 焊锡
    Epoxy,          ///< 环氧树脂
    FR4,            ///< FR4基材
    Rogers,         ///< Rogers高频材料
    PTFE            ///< PTFE（特氟龙）材料
};

/**
 * @brief Material 表示材料属性
 */
class Material {
public:
    using Id = uint64_t;

    /**
     * @brief 构造函数
     * @param name 材料名称
     * @param type 材料类型
     * @param id 材料唯一标识符
     */
    Material(const std::string& name, MaterialType type, Id id = 0);

    /**
     * @brief 析构函数
     */
    ~Material() = default;

    // ===== 属性访问 =====

    /// 获取材料ID
    Id id() const { return id_; }
    
    /// 设置材料ID
    void setId(Id id) { id_ = id; }

    /// 获取材料名称
    const std::string& name() const { return name_; }
    
    /// 设置材料名称
    void setName(const std::string& n) { name_ = n; }

    /// 获取材料类型
    MaterialType type() const { return type_; }
    
    /// 设置材料类型
    void setType(MaterialType t) { type_ = t; }

    // ===== 电气属性 =====

    /// 获取相对介电常数 (εr)
    double dielectricConstant() const { return dielectricConstant_; }
    
    /// 设置相对介电常数
    void setDielectricConstant(double dk) { dielectricConstant_ = dk; }

    /// 获取损耗角正切 (tanδ)
    double lossTangent() const { return lossTangent_; }
    
    /// 设置损耗角正切
    void setLossTangent(double tanD) { lossTangent_ = tanD; }

    /// 获取体积电阻率 (Ω·m)
    double volumeResistivity() const { return volumeResistivity_; }
    
    /// 设置体积电阻率
    void setVolumeResistivity(double rho) { volumeResistivity_ = rho; }

    /// 获取介电强度 (V/m)
    double dielectricStrength() const { return dielectricStrength_; }
    
    /// 设置介电强度
    void setDielectricStrength(double ds) { dielectricStrength_ = ds; }

    // ===== 热属性 =====

    /// 获取热导率 (W/m·K)
    double thermalConductivity() const { return thermalConductivity_; }
    
    /// 设置热导率
    void setThermalConductivity(double tc) { thermalConductivity_ = tc; }

    /// 获取热膨胀系数 (1/K)
    double thermalExpansionCoeff() const { return thermalExpansionCoeff_; }
    
    /// 设置热膨胀系数
    void setThermalExpansionCoeff(double cte) { thermalExpansionCoeff_ = cte; }

    /// 获取玻璃化转变温度 (°C)
    double glassTransitionTemp() const { return glassTransitionTemp_; }
    
    /// 设置玻璃化转变温度
    void setGlassTransitionTemp(double Tg) { glassTransitionTemp_ = Tg; }

    /// 获取最大工作温度 (°C)
    double maxOperatingTemp() const { return maxOperatingTemp_; }
    
    /// 设置最大工作温度
    void setMaxOperatingTemp(double T) { maxOperatingTemp_ = T; }

    // ===== 机械属性 =====

    /// 获取密度 (kg/m³)
    double density() const { return density_; }
    
    /// 设置密度
    void setDensity(double d) { density_ = d; }

    /// 获取弹性模量 (Pa)
    double youngsModulus() const { return youngsModulus_; }
    
    /// 设置弹性模量
    void setYoungsModulus(double E) { youngsModulus_ = E; }

    /// 获取抗拉强度 (Pa)
    double tensileStrength() const { return tensileStrength_; }
    
    /// 设置抗拉强度
    void setTensileStrength(double ts) { tensileStrength_ = ts; }

    // ===== 物理属性 =====

    /// 获取厚度 (m)
    double thickness() const { return thickness_; }
    
    /// 设置厚度
    void setThickness(double t) { thickness_ = t; }

    /// 获取颜色
    const std::string& color() const { return color_; }
    
    /// 设置颜色
    void setColor(const std::string& c) { color_ = c; }

    // ===== 预定义材料 =====

    /// 创建FR4材料
    static Material createFR4();

    /// 创建普通铜材料
    static Material createCopper();

    /// 创建Rogers RT/duroid材料
    static Material createRogers();

    // ===== 序列化 =====

    /// 获取材料类型字符串
    static const char* typeToString(MaterialType type);
    
    /// 从字符串获取材料类型
    static MaterialType stringToType(const std::string& str);

    /**
     * @brief 导出为字符串表示
     * @return 字符串表示
     */
    std::string toString() const;

private:
    Id id_;
    std::string name_;
    MaterialType type_;
    
    // 电气属性
    double dielectricConstant_;
    double lossTangent_;
    double volumeResistivity_;
    double dielectricStrength_;
    
    // 热属性
    double thermalConductivity_;
    double thermalExpansionCoeff_;
    double glassTransitionTemp_;
    double maxOperatingTemp_;
    
    // 机械属性
    double density_;
    double youngsModulus_;
    double tensileStrength_;
    
    // 物理属性
    double thickness_;
    std::string color_;
};

// 便捷类型定义
using MaterialPtr = std::shared_ptr<Material>;

} // namespace layout
} // namespace domain
} // namespace fluxcae

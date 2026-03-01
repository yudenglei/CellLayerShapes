/**
 * @file Drill.h
 * @brief Drill - 钻孔，用于PCB背钻/盲埋孔
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

/**
 * @brief DrillType 钻孔类型
 */
enum class DrillType {
    Through,      ///< 贯穿孔（通孔）
    Blind,        ///< 盲孔（从表层到内层）
    Buried,      ///< 埋孔（内层到内层）
    BackDrill    ///< 背钻（从背面钻孔）
};

/**
 * @brief Drill表示钻孔
 * 
 * 钻孔是跨层的对象，用于：
 * - 信号层的连接
 * - 背钻（去除多余的stub）
 * - 机械孔
 */
class Drill {
public:
    using Id = uint64_t;

    /**
     * @brief 构造函数
     * @param position 钻孔中心位置
     * @param diameter 钻孔直径
     * @param id 钻孔唯一标识符
     */
    Drill(const math::Point& position, int64_t diameter, Id id = 0);

    /**
     * @brief 析构函数
     */
    ~Drill() = default;

    // ===== 属性访问 =====

    /// 获取钻孔ID
    Id id() const { return id_; }
    
    /// 设置钻孔ID
    void setId(Id id) { id_ = id; }

    /// 获取钻孔中心位置
    const math::Point& position() const { return position_; }
    
    /// 设置钻孔中心位置
    void setPosition(const math::Point& pos) { position_ = pos; }

    /// 获取钻孔直径
    int64_t diameter() const { return diameter_; }
    
    /// 设置钻孔直径
    void setDiameter(int64_t d) { diameter_ = d; }

    /// 获取钻孔半径
    int64_t radius() const { return diameter_ / 2; }

    /// 获取钻孔类型
    DrillType type() const { return type_; }
    
    /// 设置钻孔类型
    void setType(DrillType type) { type_ = type; }

    /// 获取目标起始层索引
    int startLayer() const { return startLayer_; }
    
    /// 设置目标起始层索引
    void setStartLayer(int layer) { startLayer_ = layer; }

    /// 获取目标结束层索引
    int endLayer() const { return endLayer_; }
    
    /// 设置目标结束层索引
    void setEndLayer(int layer) { endLayer_ = layer; }

    // ===== 工具信息 =====

    /// 获取钻头编号
    int toolNumber() const { return toolNumber_; }
    
    /// 设置钻头编号
    void setToolNumber(int num) { toolNumber_ = num; }

    /// 获取 plating 状态（是否镀铜）
    bool isPlated() const { return isPlated_; }
    
    /// 设置 plating 状态
    void setPlated(bool plated) { isPlated_ = plated; }

    // ===== 几何计算 =====

    /// 获取钻孔边界框
    math::Box bbox() const;

    /// 检查点是否在钻孔内
    bool contains(const math::Point& pt) const;

    // ===== 序列化 =====

    /// 获取钻孔类型字符串
    static const char* typeToString(DrillType type);
    
    /// 从字符串获取钻孔类型
    static DrillType stringToType(const std::string& str);

    /**
     * @brief 导出为字符串表示
     * @return 字符串表示
     */
    std::string toString() const;

private:
    Id id_;
    math::Point position_;
    int64_t diameter_;
    DrillType type_;
    
    // 层信息
    int startLayer_;  // 起始层（从0开始）
    int endLayer_;    // 结束层
    
    // 工具信息
    int toolNumber_;
    bool isPlated_;
};

// 便捷类型定义
using DrillPtr = std::shared_ptr<Drill>;
using DrillList = std::vector<DrillPtr>;

} // namespace layout
} // namespace domain
} // namespace fluxcae

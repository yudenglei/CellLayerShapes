/**
 * @file Instance.h
 * @brief Instance - Cell的实例化引用，支持层次化布局
 * @author FluxCAE
 * @date 2024
 */
#pragma once

#include "fluxcae/foundation/math/Transform.h"
#include "fluxcae/foundation/math/Box.h"
#include <string>
#include <memory>

namespace fluxcae {

// 前向声明
class Cell;

namespace domain {
namespace layout {

/**
 * @brief Instance表示Cell的实例
 * 
 * Instance是对Cell的引用，支持：
 * - 位置变换（平移、旋转、镜像、缩放）
 * - 数组实例化（矩形阵列）
 * - 参数化实例（可变参数）
 */
class Instance {
public:
    using Id = uint64_t;
    using CellPtr = std::shared_ptr<Cell>;

    /**
     * @brief 构造函数
     * @param name 实例名称
     * @param cell 被引用的Cell
     * @param id 实例唯一标识符
     */
    Instance(const std::string& name, CellPtr cell, Id id = 0);

    /**
     * @brief 析构函数
     */
    ~Instance() = default;

    // ===== 属性访问 =====

    /// 获取实例ID
    Id id() const { return id_; }
    
    /// 获取实例名称
    const std::string& name() const { return name_; }
    
    /// 设置实例名称
    void setName(const std::string& name) { name_ = name; }

    /// 获取引用的Cell
    CellPtr cell() const { return cell_; }
    
    /// 设置引用的Cell
    void setCell(CellPtr cell) { cell_ = cell; }

    /// 获取变换矩阵
    const math::Transform& transform() const { return transform_; }
    
    /// 获取变换矩阵的可变引用
    math::Transform& transform() { return transform_; }

    /// 获取实例边界框
    math::Box bbox() const;

    // ===== 阵列配置 =====

    /// 是否为阵列实例
    bool isArray() const { return isArray_; }

    /**
     * @brief 设置阵列参数
     * @param rows 行数
     * @param cols 列数
     * @param rowStep 行间距
     * @param colStep 列间距
     */
    void setArray(size_t rows, size_t cols, 
                  const math::Vector2D& rowStep, 
                  const math::Vector2D& colStep);

    /// 获取行数
    size_t arrayRows() const { return arrayRows_; }
    
    /// 获取列数
    size_t arrayCols() const { return arrayCols_; }
    
    /// 获取行间距
    const math::Vector2D& arrayRowStep() const { return arrayRowStep_; }
    
    /// 获取列间距
    const math::Vector2D& arrayColStep() const { return arrayColStep_; }

    /**
     * @brief 获取阵列中指定位置的实例变换矩阵
     * @param row 行索引
     * @param col 列索引
     * @return 变换矩阵
     */
    math::Transform getArrayTransform(size_t row, size_t col) const;

    /// 获取实例总数（包括阵列）
    size_t totalCount() const;

    // ===== 变换操作 =====

    /**
     * @brief 设置平移
     * @param dx X方向平移量
     * @param dy Y方向平移量
     */
    void setTranslation(int64_t dx, int64_t dy);

    /**
     * @brief 设置旋转
     * @param angle 旋转角度（角度制）
     * @param center 旋转中心
     */
    void setRotation(double angle, const math::Point& center = math::Point());

    /**
     * @brief 设置镜像
     * @param axis 镜像轴：'x'或'y'
     * @param axisPos 镜像轴位置
     */
    void setMirror(char axis, int64_t axisPos = 0);

    /**
     * @brief 应用变换
     * @param transform 变换矩阵
     */
    void applyTransform(const math::Transform& transform);

    // ===== 序列化 =====

    /**
     * @brief 导出为字符串表示
     * @return 字符串表示
     */
    std::string toString() const;

private:
    Id id_;
    std::string name_;
    CellPtr cell_;
    math::Transform transform_;
    
    // 阵列参数
    bool isArray_;
    size_t arrayRows_;
    size_t arrayCols_;
    math::Vector2D arrayRowStep_;
    math::Vector2D arrayColStep_;
};

} // namespace layout
} // namespace domain
} // namespace fluxcae

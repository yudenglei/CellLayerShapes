/**
 * @file Cell.h
 * @brief Cell - 布局中的基本单元，包含了多个图元的集合
 * @author FluxCAE
 * @date 2024
 */
#pragma once

#include "fluxcae/foundation/math/Point.h"
#include "fluxcae/foundation/math/Box.h"
#include "fluxcae/foundation/math/Transform.h"
#include "fluxcae/foundation/utils/Logger.h"
#include <string>
#include <vector>
#include <memory>
#include <unordered_map>

namespace fluxcae {

// 前向声明
class Instance;
class Shape;
class Via;
class Drill;

namespace domain {
namespace layout {

/**
 * @brief Cell是布局中的基本单元
 * 
 * Cell可以包含：
 * - 静态图元（形状、路径、多边形）
 * - 子单元实例（Instance）
 * - 通孔（Via）
 * - 钻孔（Drill）
 * 
 * Cell支持层次化设计，支持cell嵌套
 */
class Cell {
public:
    using Id = uint64_t;
    using ShapePtr = std::shared_ptr<Shape>;
    using InstancePtr = std::shared_ptr<Instance>;
    using ViaPtr = std::shared_ptr<Via>;
    using DrillPtr = std::shared_ptr<Drill>;
    using ShapeList = std::vector<ShapePtr>;
    using InstanceList = std::vector<InstancePtr>;
    using ViaList = std::vector<ViaPtr>;
    using DrillList = std::vector<DrillPtr>;

    /**
     * @brief 构造函数
     * @param name Cell名称
     * @param id Cell唯一标识符
     */
    Cell(const std::string& name, Id id = 0);

    /**
     * @brief 析构函数
     */
    ~Cell() = default;

    // ===== 属性访问 =====
    
    /// 获取Cell ID
    Id id() const { return id_; }
    
    /// 获取Cell名称
    const std::string& name() const { return name_; }
    
    /// 设置Cell名称
    void setName(const std::string& name) { name_ = name; }

    /// 获取Cell边界框
    const math::Box& bbox() const { return bbox_; }

    /// 判断是否为空Cell
    bool isEmpty() const {
        return shapes_.empty() && instances_.empty() && vias_.empty() && drills_.empty();
    }

    // ===== 图元管理 =====

    /**
     * @brief 添加形状
     * @param shape 形状指针
     * @return 添加成功返回true
     */
    bool addShape(ShapePtr shape);

    /**
     * @brief 移除形状
     * @param shape 形状指针
     * @return 移除成功返回true
     */
    bool removeShape(ShapePtr shape);

    /// 获取所有形状
    const ShapeList& shapes() const { return shapes_; }
    
    /// 获取形状数量
    size_t shapeCount() const { return shapes_.size(); }

    // ===== 实例管理 =====

    /**
     * @brief 添加子单元实例
     * @param instance 实例指针
     * @return 添加成功返回true
     */
    bool addInstance(InstancePtr instance);

    /**
     * @brief 移除子单元实例
     * @param instance 实例指针
     * @return 移除成功返回true
     */
    bool removeInstance(InstancePtr instance);

    /// 获取所有实例
    const InstanceList& instances() const { return instances_; }
    
    /// 获取实例数量
    size_t instanceCount() const { return instances_.size(); }

    /**
     * @brief 根据名称查找实例
     * @param name 实例名称
     * @return 找到返回实例指针，否则返回nullptr
     */
    InstancePtr findInstance(const std::string& name) const;

    // ===== 通孔管理 =====

    /**
     * @brief 添加通孔
     * @param via 通孔指针
     * @return 添加成功返回true
     */
    bool addVia(ViaPtr via);

    /**
     * @brief 移除通孔
     * @param via 通孔指针
     * @return 移除成功返回true
     */
    bool removeVia(ViaPtr via);

    /// 获取所有通孔
    const ViaList& vias() const { return vias_; }
    
    /// 获取通孔数量
    size_t viaCount() const { return vias_.size(); }

    // ===== 钻孔管理 =====

    /**
     * @brief 添加钻孔
     * @param drill 钻孔指针
     * @return 添加成功返回true
     */
    bool addDrill(DrillPtr drill);

    /**
     * @brief 移除钻孔
     * @param drill 钻孔指针
     * @return 移除成功返回true
     */
    bool removeDrill(DrillPtr drill);

    /// 获取所有钻孔
    const DrillList& drills() const { return drills_; }
    
    /// 获取钻孔数量
    size_t drillCount() const { return drills_.size(); }

    // ===== 变换操作 =====

    /**
     * @brief 应用变换矩阵到所有图元
     * @param transform 变换矩阵
     */
    void transform(const math::Transform& transform);

    /**
     * @brief 清除所有图元
     */
    void clear();

    // ===== 序列化 =====

    /**
     * @brief 导出为字符串表示
     * @return 字符串表示
     */
    std::string toString() const;

    /**
     * @brief 更新边界框
     */
    void updateBBox();

private:
    Id id_;
    std::string name_;
    math::Box bbox_;
    
    ShapeList shapes_;
    InstanceList instances_;
    ViaList vias_;
    DrillList drills_;
    
    // 实例名称索引
    std::unordered_map<std::string, InstancePtr> instanceNameIndex_;
};

} // namespace layout
} // namespace domain
} // namespace fluxcae

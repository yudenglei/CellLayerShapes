#ifndef FLUXCAE_LAYER_H
#define FLUXCAE_LAYER_H

#include "fluxcae/foundation/math/Box.h"
#include "fluxcae/domain/layout/Shape.h"
#include "fluxcae/domain/layout/SpatialIndex.h"
#include <memory>
#include <string>
#include <vector>
#include <map>

namespace fluxcae {

/// @brief 层类型
enum class LayerType {
    Signal,     // 信号层
    Plane,      // 平面层（电源/地）
    Dielectric, // 介质层
    Mask,       // 阻焊层/丝印
    Silk,       // 丝印层
    Drill,      // 钻孔层
    Cut,        // 切割层
    User        // 用户自定义
};

/// @brief 物理层
class Layer {
public:
    using ShapeMap = std::map<ShapeID, std::unique_ptr<Shape>>;
    
private:
    LayerID id_;
    std::string name_;
    
    // GDS/OASIS兼容
    int gds_layer_;
    int gds_datatype_;
    
    // 层属性
    LayerType type_;
    int z_position_;        // Z轴位置（纳米单位）
    int thickness_;        // 厚度
    
    // 空间索引
    std::unique_ptr<SpatialIndex> spatial_index_;
    
    // 形状存储
    ShapeMap shapes_;
    
    // 可见性（用于渲染）
    bool visible_ = true;
    int display_order_ = 0;
    
public:
    Layer(LayerID id, int gds_layer, int gds_datatype);
    ~Layer();
    
    // 拷贝禁用
    Layer(const Layer&) = delete;
    Layer& operator=(const Layer&) = delete;
    
    // 移动语义
    Layer(Layer&&) noexcept;
    Layer& operator=(Layer&&) noexcept;
    
    // ===== 属性访问 =====
    LayerID id() const { return id_; }
    const std::string& name() const { return name_; }
    void setName(const std::string& name) { name_ = name; }
    
    int gdsLayer() const { return gds_layer_; }
    int gdsDatatype() const { return gds_datatype_; }
    
    LayerType type() const { return type_; }
    void setType(LayerType type) { type_ = type; }
    
    int zPosition() const { return z_position_; }
    void setZPosition(int z) { z_position_ = z; }
    
    int thickness() const { return thickness_; }
    void setThickness(int t) { thickness_ = t; }
    
    // ===== 可见性 =====
    bool visible() const { return visible_; }
    void setVisible(bool v) { visible_ = v; }
    
    int displayOrder() const { return display_order_; }
    void setDisplayOrder(int order) { display_order_ = order; }
    
    // ===== 形状操作 =====
    ShapeID addShape(std::unique_ptr<Shape> shape);
    void removeShape(ShapeID id);
    Shape* getShape(ShapeID id);
    const Shape* getShape(ShapeID id) const;
    
    ShapeMap& shapes() { return shapes_; }
    const ShapeMap& shapes() const { return shapes_; }
    
    size_t shapeCount() const { return shapes_.size(); }
    
    // ===== 空间查询 =====
    SpatialIndex* spatialIndex() { return spatial_index_.get(); }
    const SpatialIndex* spatialIndex() const { return spatial_index_.get(); }
    
    std::vector<ShapeID> query(const Box& region) const;
    std::vector<ShapeID> query(const Point& point) const;
    
    // ===== 批量操作 =====
    void clear();
    void reserve(size_t count);
};

} // namespace fluxcae

#endif // FLUXCAE_LAYER_H

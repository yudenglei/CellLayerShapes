#ifndef FLUXCAE_SHAPE_H
#define FLUXCAE_SHAPE_H

#include "fluxcae/foundation/math/Point.h"
#include "fluxcae/foundation/math/Box.h"
#include "fluxcae/foundation/math/Polygon.h"
#include <memory>
#include <variant>

namespace fluxcae {

/// @brief 形状类型
enum class ShapeType {
    Box,
    Polygon,
    Path,
    Text,
    Arc
};

/// @brief 形状基类
class Shape {
public:
    virtual ~Shape() = default;
    
    virtual ShapeType type() const = 0;
    virtual Box boundingBox() const = 0;
    virtual bool isEmpty() const = 0;
    
    // 克隆
    virtual std::unique_ptr<Shape> clone() const = 0;
    
    // 变换
    virtual void transform(const class Transform2D& t) = 0;
    virtual std::unique_ptr<Shape> transformed(const Transform2D& t) const = 0;
    
    // 属性
    virtual int layerId() const = 0;
    virtual void setLayerId(int id) = 0;
    
    virtual uint64_t netId() const = 0;
    virtual void setNetId(uint64_t id) = 0;
};

/// @brief 矩形形状
class BoxShape : public Shape {
private:
    Box box_;
    int layer_id_ = 0;
    uint64_t net_id_ = 0;
    
public:
    explicit BoxShape() = default;
    explicit BoxShape(const Box& box) : box_(box) {}
    explicit BoxShape(int64_t x1, int64_t y1, int64_t x2, int64_t y2) 
        : box_(x1, y1, x2, y2) {}
    
    ShapeType type() const override { return ShapeType::Box; }
    Box boundingBox() const override { return box_; }
    bool isEmpty() const override { return box_.isEmpty(); }
    
    const Box& box() const { return box_; }
    Box& box() { return box_; }
    
    std::unique_ptr<Shape> clone() const override {
        return std::make_unique<BoxShape>(*this);
    }
    
    void transform(const Transform2D& t) override;
    std::unique_ptr<Shape> transformed(const Transform2D& t) const override;
    
    int layerId() const override { return layer_id_; }
    void setLayerId(int id) override { layer_id_ = id; }
    
    uint64_t netId() const override { return net_id_; }
    void setNetId(uint64_t id) override { net_id_ = id; }
};

/// @brief 多边形形状
class PolygonShape : public Shape {
private:
    Polygon polygon_;
    int layer_id_ = 0;
    uint64_t net_id_ = 0;
    
public:
    explicit PolygonShape() = default;
    explicit PolygonShape(Polygon poly) : polygon_(std::move(poly)) {}
    
    ShapeType type() const override { return ShapeType::Polygon; }
    Box boundingBox() const override { return polygon_.boundingBox(); }
    bool isEmpty() const override { return polygon_.isEmpty(); }
    
    const Polygon& polygon() const { return polygon_; }
    Polygon& polygon() { return polygon_; }
    
    std::unique_ptr<Shape> clone() const override {
        return std::make_unique<PolygonShape>(*this);
    }
    
    void transform(const Transform2D& t) override;
    std::unique_ptr<Shape> transformed(const Transform2D& t) const override;
    
    int layerId() const override { return layer_id_; }
    void setLayerId(int id) override { layer_id_ = id; }
    
    uint64_t netId() const override { return net_id_; }
    void setNetId(uint64_t id) override { net_id_ = id; }
};

/// @brief 路径形状
class PathShape : public Shape {
private:
    Path path_;
    int layer_id_ = 0;
    uint64_t net_id_ = 0;
    
public:
    explicit PathShape() = default;
    explicit PathShape(Path p) : path_(std::move(p)) {}
    
    ShapeType type() const override { return ShapeType::Path; }
    Box boundingBox() const override { return path_.boundingBox(); }
    bool isEmpty() const override { return path_.isEmpty(); }
    
    const Path& path() const { return path_; }
    Path& path() { return path_; }
    
    std::unique_ptr<Shape> clone() const override {
        return std::make_unique<PathShape>(*this);
    }
    
    void transform(const Transform2D& t) override;
    std::unique_ptr<Shape> transformed(const Transform2D& t) const override;
    
    int layerId() const override { return layer_id_; }
    void setLayerId(int id) override { layer_id_ = id; }
    
    uint64_t netId() const override { return net_id_; }
    void setNetId(uint64_t id) override { net_id_ = id; }
};

// 类型别名
using ShapePtr = std::unique_ptr<Shape>;
using ShapeVariant = std::variant<BoxShape, PolygonShape, PathShape>;

} // namespace fluxcae

#endif // FLUXCAE_SHAPE_H

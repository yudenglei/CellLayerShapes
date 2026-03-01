#ifndef FLUXCAE_POLYGON_H
#define FLUXCAE_POLYGON_H

#include "fluxcae/foundation/math/Point.h"
#include "fluxcae/foundation/math/Box.h"
#include <vector>
#include <algorithm>

namespace fluxcae {

/// @brief 多边形类
class Polygon {
public:
    using coordinate_type = Point::coordinate_type;
    using Points = std::vector<Point>;

private:
    Points points_;
    Points holes_;  // 简单实现：只支持一个孔
    bool is_hole_ = false;

public:
    Polygon() = default;
    
    explicit Polygon(Points points) : points_(std::move(points)) {}
    
    // 顶点操作
    const Points& points() const { return points_; }
    Points& points() { return points_; }
    
    size_t numPoints() const { return points_.size(); }
    
    void addPoint(const Point& p) { points_.push_back(p); }
    void addPoints(const Points& pts) {
        points_.insert(points_.end(), pts.begin(), pts.end());
    }
    
    // 包围盒
    Box boundingBox() const {
        if (points_.empty()) return Box();
        
        coordinate_type min_x = points_[0].x;
        coordinate_type max_x = points_[0].x;
        coordinate_type min_y = points_[0].y;
        coordinate_type max_y = points_[0].y;
        
        for (const auto& p : points_) {
            min_x = std::min(min_x, p.x);
            max_x = std::max(max_x, p.x);
            min_y = std::min(min_y, p.y);
            max_y = std::max(max_y, p.y);
        }
        
        return Box(min_x, min_y, max_x, max_y);
    }
    
    // 是否为空
    bool isEmpty() const { return points_.empty(); }
    
    // 是否是孔
    bool isHole() const { return is_hole_; }
    void setHole(bool hole) { is_hole_ = hole; }
    
    // 面积（简化实现，假设非自交）
    double area() const;
    
    // 简单点测试（射线法）
    bool contains(const Point& p) const;
    
    // 变换
    void transform(const class Transform2D& t);
    Polygon transformed(const class Transform2D& t) const;
    
    // 运算符
    bool operator==(const Polygon& other) const {
        return points_ == other.points_;
    }
};

/// @brief 路径类
class Path {
public:
    enum class CapStyle { Butt, Round, Square };
    enum class JoinStyle { Miter, Round, Bevel };

private:
    std::vector<Point> points_;
    coordinate_type width_ = 0;
    CapStyle start_cap_ = CapStyle::Butt;
    CapStyle end_cap_ = CapStyle::Butt;
    JoinStyle join_style_ = JoinStyle::Miter;

public:
    Path() = default;
    
    Path(std::vector<Point> points, coordinate_type width)
        : points_(std::move(points)), width_(width) {}
    
    const std::vector<Point>& points() const { return points_; }
    std::vector<Point>& points() { return points_; }
    
    size_t numPoints() const { return points_.size(); }
    coordinate_type width() const { return width_; }
    void setWidth(coordinate_type w) { width_ = w; }
    
    void addPoint(const Point& p) { points_.push_back(p); }
    
    CapStyle startCap() const { return start_cap_; }
    CapStyle endCap() const { return end_cap_; }
    void setStartCap(CapStyle s) { start_cap_ = s; }
    void setEndCap(CapStyle e) { end_cap_ = e; }
    
    JoinStyle joinStyle() const { return join_style_; }
    void setJoinStyle(JoinStyle j) { join_style_ = j; }
    
    Box boundingBox() const;
    bool isEmpty() const { return points_.empty(); }
    
    void transform(const class Transform2D& t);
    Path transformed(const class Transform2D& t) const;
};

/// @brief 文本类
class Text {
private:
    Point position_;
    std::string text_;
    coordinate_type height_ = 1000;
    double rotation_ = 0.0;  // 度
    bool mirrored_ = false;

public:
    Text() = default;
    Text(const Point& pos, const std::string& text, coordinate_type height = 1000)
        : position_(pos), text_(text), height_(height) {}
    
    const Point& position() const { return position_; }
    Point& position() { return position_; }
    
    const std::string& text() const { return text_; }
    std::string& text() { return text_; }
    
    coordinate_type height() const { return height_; }
    void setHeight(coordinate_type h) { height_ = h; }
    
    double rotation() const { return rotation_; }
    void setRotation(double r) { rotation_ = r; }
    
    bool mirrored() const { return mirrored_; }
    void setMirrored(bool m) { mirrored_ = m; }
    
    Box boundingBox() const;
    
    void transform(const class Transform2D& t);
    Text transformed(const class Transform2D& t) const;
};

} // namespace fluxcae

#endif // FLUXCAE_POLYGON_H

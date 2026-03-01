#ifndef FLUXCAE_BOX_H
#define FLUXCAE_BOX_H

#include "fluxcae/foundation/math/Point.h"
#include <algorithm>
#include <functional>

namespace fluxcae {

/// @brief 轴对齐矩形包围盒
class Box {
public:
    using coordinate_type = Point::coordinate_type;
    
    coordinate_type x1, y1;
    coordinate_type x2, y2;
    
    constexpr Box() noexcept : x1(0), y1(0), x2(0), y2(0) {}
    constexpr Box(coordinate_type x1, coordinate_type y1, 
                  coordinate_type x2, coordinate_type y2) noexcept
        : x1(x1), y1(y1), x2(x2), y2(y2) { normalize(); }
    
    constexpr Box(const Point& p1, const Point& p2) noexcept
        : x1(p1.x), y1(p1.y), x2(p2.x), y2(p2.y) { normalize(); }
    
    void normalize() noexcept {
        if (x1 > x2) std::swap(x1, x2);
        if (y1 > y2) std::swap(y1, y2);
    }
    
    coordinate_type width() const noexcept { return x2 - x1; }
    coordinate_type height() const noexcept { return y2 - y1; }
    coordinate_type area() const noexcept { return width() * height(); }
    
    Point center() const noexcept { return Point((x1 + x2) / 2, (y1 + y2) / 2); }
    bool isEmpty() const noexcept { return x1 == x2 || y1 == y2; }
    
    bool contains(const Point& p) const noexcept {
        return p.x >= x1 && p.x <= x2 && p.y >= y1 && p.y <= y2;
    }
    
    bool contains(const Box& b) const noexcept {
        return b.x1 >= x1 && b.x2 <= x2 && b.y1 >= y1 && b.y2 <= y2;
    }
    
    bool intersects(const Box& b) const noexcept {
        return !(b.x1 > x2 || b.x2 < x1 || b.y1 > y2 || b.y2 < y1);
    }
    
    static Box merge(const Box& a, const Box& b) noexcept {
        return Box(std::min(a.x1, b.x1), std::min(a.y1, b.y1),
                   std::max(a.x2, b.x2), std::max(a.y2, b.y2));
    }
    
    Box intersection(const Box& b) const noexcept {
        if (!intersects(b)) return Box();
        return Box(std::max(x1, b.x1), std::max(y1, b.y1),
                   std::min(x2, b.x2), std::min(y2, b.y2));
    }
    
    Box enlarge(coordinate_type d) const noexcept {
        return Box(x1 - d, y1 - d, x2 + d, y2 + d);
    }
    
    Box translate(const Point& delta) const noexcept {
        return Box(x1 + delta.x, y1 + delta.y, x2 + delta.x, y2 + delta.y);
    }
    
    bool operator==(const Box& other) const noexcept {
        return x1 == other.x1 && y1 == other.y1 && x2 == other.x2 && y2 == other.y2;
    }
};

class Box3D {
public:
    using coordinate_type = Point3D::coordinate_type;
    
    coordinate_type x1, y1, z1, x2, y2, z2;
    
    constexpr Box3D() noexcept : x1(0), y1(0), z1(0), x2(0), y2(0), z2(0) {}
    constexpr Box3D(coordinate_type x1, coordinate_type y1, coordinate_type z1,
                    coordinate_type x2, coordinate_type y2, coordinate_type z2) noexcept
        : x1(x1), y1(y1), z1(z1), x2(x2), y2(y2), z2(z2) { normalize(); }
    
    void normalize() noexcept {
        if (x1 > x2) std::swap(x1, x2);
        if (y1 > y2) std::swap(y1, y2);
        if (z1 > z2) std::swap(z1, z2);
    }
    
    Box to2D() const noexcept { return Box(x1, y1, x2, y2); }
};

inline std::ostream& operator<<(std::ostream& os, const Box& b) {
    os << "Box(" << b.x1 << ", " << b.y1 << ", " << b.x2 << ", " << b.y2 << ")";
    return os;
}

} // namespace fluxcae

#endif

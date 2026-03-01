#ifndef FLUXCAE_POINT_H
#define FLUXCAE_POINT_H

#include <cstdint>
#include <ostream>
#include <type_traits>

namespace fluxcae {

/// @brief 2D点坐标类
/// @details 使用整数坐标（纳米级单位）避免浮点误差
class Point {
public:
    using coordinate_type = int64_t;
    
    coordinate_type x;
    coordinate_type y;
    
    constexpr Point() noexcept : x(0), y(0) {}
    constexpr Point(coordinate_type x, coordinate_type y) noexcept 
        : x(x), y(y) {}
    
    // 运算符重载
    Point operator+(const Point& other) const noexcept {
        return Point(x + other.x, y + other.y);
    }
    
    Point operator-(const Point& other) const noexcept {
        return Point(x - other.x, y - other.y);
    }
    
    Point& operator+=(const Point& other) noexcept {
        x += other.x;
        y += other.y;
        return *this;
    }
    
    Point& operator-=(const Point& other) noexcept {
        x -= other.x;
        y -= other.y;
        return *this;
    }
    
    Point operator*(coordinate_type scalar) const noexcept {
        return Point(x * scalar, y * scalar);
    }
    
    Point operator/(coordinate_type scalar) const noexcept {
        return Point(x / scalar, y / scalar);
    }
    
    bool operator==(const Point& other) const noexcept {
        return x == other.x && y == other.y;
    }
    
    bool operator!=(const Point& other) const noexcept {
        return !(*this == other);
    }
    
    bool operator<(const Point& other) const noexcept {
        if (x != other.x) return x < other.x;
        return y < other.y;
    }
    
    // 距离平方（用于空间查询优化）
    coordinate_type distanceSquared(const Point& other) const noexcept {
        coordinate_type dx = x - other.x;
        coordinate_type dy = y - other.y;
        return dx * dx + dy * dy;
    }
    
    // 距离
    double distance(const Point& other) const noexcept {
        return std::sqrt(static_cast<double>(distanceSquared(other)));
    }
    
    // 线性插值
    Point lerp(const Point& other, double t) const noexcept {
        return Point(
            static_cast<coordinate_type>(x + (other.x - x) * t),
            static_cast<coordinate_type>(y + (other.y - y) * t)
        );
    }
    
    void swap(Point& other) noexcept {
        std::swap(x, other.x);
        std::swap(y, other.y);
    }
    
    // 归零
    void clear() noexcept {
        x = 0;
        y = 0;
    }
    
    // 是否为零
    bool isZero() const noexcept {
        return x == 0 && y == 0;
    }
};

/// @brief 3D点坐标类
class Point3D {
public:
    using coordinate_type = int64_t;
    
    coordinate_type x;
    coordinate_type y;
    coordinate_type z;
    
    constexpr Point3D() noexcept : x(0), y(0), z(0) {}
    constexpr Point3D(coordinate_type x, coordinate_type y, coordinate_type z) noexcept
        : x(x), y(y), z(z) {}
    
    Point3D(const Point& p, coordinate_type z = 0) noexcept
        : x(p.x), y(p.y), z(z) {}
    
    Point3D operator+(const Point3D& other) const noexcept {
        return Point3D(x + other.x, y + other.y, z + other.z);
    }
    
    Point3D operator-(const Point3D& other) const noexcept {
        return Point3D(x - other.x, y - other.y, z - other.z);
    }
    
    Point3D& operator+=(const Point3D& other) noexcept {
        x += other.x;
        y += other.y;
        z += other.z;
        return *this;
    }
    
    Point3D operator*(coordinate_type scalar) const noexcept {
        return Point3D(x * scalar, y * scalar, z * scalar);
    }
    
    bool operator==(const Point3D& other) const noexcept {
        return x == other.x && y == other.y && z == other.z;
    }
    
    bool operator!=(const Point3D& other) const noexcept {
        return !(*this == other);
    }
    
    // 距离平方
    coordinate_type distanceSquared(const Point3D& other) const noexcept {
        coordinate_type dx = x - other.x;
        coordinate_type dy = y - other.y;
        coordinate_type dz = z - other.z;
        return dx * dx + dy * dy + dz * dz;
    }
    
    // 转换为2D点
    Point to2D() const noexcept {
        return Point(x, y);
    }
};

// 便捷类型别名
using Point2D = Point;

// 输出流支持
inline std::ostream& operator<<(std::ostream& os, const Point& p) {
    os << "Point(" << p.x << ", " << p.y << ")";
    return os;
}

inline std::ostream& operator<<(std::ostream& os, const Point3D& p) {
    os << "Point3D(" << p.x << ", " << p.y << ", " << p.z << ")";
    return os;
}

} // namespace fluxcae

#endif // FLUXCAE_POINT_H

#ifndef FLUXCAE_TRANSFORM_H
#define FLUXCAE_TRANSFORM_H

#include "fluxcae/foundation/math/Point.h"
#include <cmath>
#include <array>

namespace fluxcae {

/// @brief 2D变换矩阵类
/// @details 支持平移、旋转、缩放、镜像等2D仿射变换
class Transform2D {
public:
    using value_type = double;
    
private:
    // 3x3齐次坐标矩阵 [a b c]
    //                     [d e f]
    //                     [0 0 1]
    std::array<value_type, 6> m_;  // 只存储前两行，a,b,c,d,e,f

public:
    Transform2D() : m_{{1, 0, 0, 0, 1, 0}} {}  // 单位矩阵
    
    // 从矩阵元素构造
    Transform2D(value_type a, value_type b, value_type c,
                value_type d, value_type e, value_type f)
        : m_{{a, b, c, d, e, f}} {}
    
    // 矩阵元素访问
    value_type a() const { return m_[0]; }
    value_type b() const { return m_[1]; }
    value_type c() const { return m_[2]; }  // tx
    value_type d() const { return m_[3]; }
    value_type e() const { return m_[4]; }
    value_type f() const { return m_[5]; }  // ty
    
    // 便捷构造方法
    static Transform2D identity() { return Transform2D(); }
    
    // 平移
    static Transform2D translation(value_type tx, value_type ty) {
        return Transform2D(1, 0, tx, 0, 1, ty);
    }
    
    static Transform2D translation(const Vector2D& v) {
        return translation(v.x, v.y);
    }
    
    // 缩放
    static Transform2D scaling(value_type sx, value_type sy) {
        return Transform2D(sx, 0, 0, 0, sy, 0);
    }
    
    static Transform2D scaling(value_type s) {
        return scaling(s, s);
    }
    
    // 旋转（弧度）
    static Transform2D rotation(value_type angle_rad) {
        value_type c = std::cos(angle_rad);
        value_type s = std::sin(angle_rad);
        return Transform2D(c, -s, 0, s, c, 0);
    }
    
    // 旋转（角度）
    static Transform2D rotationDegrees(value_type angle_deg) {
        return rotation(angle_deg * 3.14159265358979323846 / 180.0);
    }
    
    // 镜像
    static Transform2D mirrorX() { return Transform2D(1, 0, 0, 0, -1, 0); }
    static Transform2D mirrorY() { return Transform2D(-1, 0, 0, 0, 1, 0); }
    
    // 组合变换
    Transform2D operator*(const Transform2D& other) const {
        // this * other: 先应用other，再应用this
        return Transform2D(
            a() * other.a() + b() * other.d(),
            a() * other.b() + b() * other.e(),
            a() * other.c() + b() * other.f() + c(),
            d() * other.a() + e() * other.d(),
            d() * other.b() + e() * other.e(),
            d() * other.c() + e() * other.f() + f()
        );
    }
    
    Transform2D& operator*=(const Transform2D& other) {
        *this = *this * other;
        return *this;
    }
    
    // 逆矩阵
    Transform2D inverse() const {
        value_type det = a() * e() - b() * d();
        if (std::abs(det) < 1e-10) {
            return identity();  // 退化情况
        }
        
        value_type inv_det = 1.0 / det;
        return Transform2D(
            e() * inv_det,
            -b() * inv_det,
            (b() * f() - e() * c()) * inv_det,
            -d() * inv_det,
            a() * inv_det,
            (d() * c() - a() * f()) * inv_det
        );
    }
    
    // 点变换
    Point transformPoint(const Point& p) const {
        value_type x = static_cast<value_type>(p.x);
        value_type y = static_cast<value_type>(p.y);
        return Point(
            static_cast<Point::coordinate_type>(a() * x + b() * y + c()),
            static_cast<Point::coordinate_type>(d() * x + e() * y + f())
        );
    }
    
    // 向量变换（不考虑平移）
    Vector2D transformVector(const Vector2D& v) const {
        value_type x = static_cast<value_type>(v.x);
        value_type y = static_cast<value_type>(v.y);
        return Vector2D(
            static_cast<Point::coordinate_type>(a() * x + b() * y),
            static_cast<Point::coordinate_type>(d() * x + e() * y)
        );
    }
    
    // 包围盒变换
    Box transformBox(const Box& box) const;
    
    // 运算符
    bool operator==(const Transform2D& other) const {
        for (int i = 0; i < 6; ++i) {
            if (std::abs(m_[i] - other.m_[i]) > 1e-10) return false;
        }
        return true;
    }
    
    bool isIdentity() const {
        return *this == identity();
    }
    
    // 判断是否只包含平移和旋转（保角变换）
    bool isRigid() const {
        value_type det = a() * e() - b() * d();
        return std::abs(det - 1.0) < 1e-10 &&
               std::abs(a() * a() + d() * d() - 1.0) < 1e-10;
    }
};

using Transformation2D = Transform2D;

} // namespace fluxcae

#endif // FLUXCAE_TRANSFORM_H

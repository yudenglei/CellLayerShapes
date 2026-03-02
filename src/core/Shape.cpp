/**
 * @file Shape.cpp
 * @brief Shape implementation
 */
#include "fluxcae/domain/layout/Shape.h"
#include "fluxcae/foundation/math/Transform.h"

namespace fluxcae {
namespace domain {
namespace layout {

// Shape base
Shape::Shape(ShapeType type, Id id) : type_(type), id_(id) {}
Shape::~Shape() = default;

void Shape::transform(const math::Transform& t) {
    // Override in derived classes
}

math::Box Shape::bbox() const {
    return math::Box();
}

// BoxShape
BoxShape::BoxShape(const math::Box& box, Id id)
    : Shape(ShapeType::Box, id), box_(box) {}

BoxShape::~BoxShape() = default;

void BoxShape::transform(const math::Transform& t) {
    box_ = t.transform(box_);
}

math::Box BoxShape::bbox() const {
    return box_;
}

// PolygonShape
PolygonShape::PolygonShape(const math::Polygon& polygon, Id id)
    : Shape(ShapeType::Polygon, id), polygon_(polygon) {}

PolygonShape::~PolygonShape() = default;

void PolygonShape::transform(const math::Transform& t) {
    polygon_ = t.transform(polygon_);
}

math::Box PolygonShape::bbox() const {
    return polygon_.bbox();
}

// PathShape
PathShape::PathShape(const math::Path& path, Id id)
    : Shape(ShapeType::Path, id), path_(path) {}

PathShape::~PathShape() = default;

void PathShape::transform(const math::Transform& t) {
    path_ = t.transform(path_);
}

math::Box PathShape::bbox() const {
    return path_.bbox();
}

// TextShape
TextShape::TextShape(const std::string& text, const math::Point& position, Id id)
    : Shape(ShapeType::Text, id), text_(text), position_(position) {}

TextShape::~TextShape() = default;

void TextShape::transform(const math::Transform& t) {
    position_ = t.transform(position_);
}

math::Box TextShape::bbox() const {
    return math::Box(position_.x, position_.y, position_.x + 100, position_.y + 50);
}

} // namespace layout
} // namespace domain
} // namespace fluxcae

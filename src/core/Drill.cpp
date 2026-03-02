/**
 * @file Drill.cpp
 * @brief Drill implementation
 */
#include "fluxcae/domain/layout/Drill.h"

namespace fluxcae {
namespace domain {
namespace layout {

Drill::Drill(const math::Point& position, int64_t diameter, Id id)
    : id_(id), position_(position), diameter_(diameter),
      type_(DrillType::Through), startLayer_(0), endLayer_(0),
      toolNumber_(0), isPlated_(true) {
}

Drill::~Drill() = default;

math::Box Drill::bbox() const {
    int64_t r = diameter_ / 2;
    return math::Box(
        position_.x - r, position_.y - r,
        position_.x + r, position_.y + r
    );
}

bool Drill::contains(const math::Point& pt) const {
    int64_t dx = pt.x - position_.x;
    int64_t dy = pt.y - position_.y;
    int64_t r = diameter_ / 2;
    return (dx * dx + dy * dy) <= (r * r);
}

const char* Drill::typeToString(DrillType type) {
    switch (type) {
        case DrillType::Through: return "Through";
        case DrillType::Blind: return "Blind";
        case DrillType::Buried: return "Buried";
        case DrillType::BackDrill: return "BackDrill";
        default: return "Unknown";
    }
}

DrillType Drill::stringToType(const std::string& str) {
    if (str == "Through") return DrillType::Through;
    if (str == "Blind") return DrillType::Blind;
    if (str == "Buried") return DrillType::Buried;
    if (str == "BackDrill") return DrillType::BackDrill;
    return DrillType::Through;
}

std::string Drill::toString() const {
    return "Drill(" + std::to_string(position_.x) + ", " + 
           std::to_string(position_.y) + ", " + 
           std::to_string(diameter_) + ")";
}

} // namespace layout
} // namespace domain
} // namespace fluxcae

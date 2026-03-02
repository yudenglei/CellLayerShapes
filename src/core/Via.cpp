/**
 * @file Via.cpp
 * @brief Via implementation
 */
#include "fluxcae/domain/layout/Via.h"
#include <cmath>

namespace fluxcae {
namespace domain {
namespace layout {

Via::Via(const math::Point& position, Id id)
    : id_(id), position_(position), 
      topDiameter_(100000), bottomDiameter_(100000),
      type_(ViaType::Through), startLayer_(0), endLayer_(0) {
}

Via::~Via() = default;

math::Box Via::bbox() const {
    int64_t r = topDiameter_ / 2;
    return math::Box(
        position_.x - r, position_.y - r,
        position_.x + r, position_.y + r
    );
}

bool Via::contains(const math::Point& pt) const {
    int64_t dx = pt.x - position_.x;
    int64_t dy = pt.y - position_.y;
    int64_t r = topDiameter_ / 2;
    return (dx * dx + dy * dy) <= (r * r);
}

void Via::setTopPad(int64_t diameter) {
    topDiameter_ = diameter;
}

void Via::setBottomPad(int64_t diameter) {
    bottomDiameter_ = diameter;
}

void Via::setStartLayer(int layer) {
    startLayer_ = layer;
}

void Via::setEndLayer(int layer) {
    endLayer_ = layer;
}

void Via::setType(ViaType type) {
    type_ = type;
}

const char* Via::typeToString(ViaType type) {
    switch (type) {
        case ViaType::Through: return "Through";
        case ViaType::Blind: return "Blind";
        case ViaType::Buried: return "Buried";
        case ViaType::Micro: return "Micro";
        default: return "Unknown";
    }
}

ViaType Via::stringToType(const std::string& str) {
    if (str == "Through") return ViaType::Through;
    if (str == "Blind") return ViaType::Blind;
    if (str == "Buried") return ViaType::Buried;
    if (str == "Micro") return ViaType::Micro;
    return ViaType::Through;
}

// PadstackDef
PadstackDef::PadstackDef(const std::string& name, Id id)
    : id_(id), name_(name), 
      topDiameter_(100000), bottomDiameter_(100000), 
      drillDiameter_(60000) {
}

PadstackDef::~PadstackDef() = default;

void PadstackDef::addLayer(int layerIndex, int64_t padDiameter) {
    padLayers_[layerIndex] = padDiameter;
}

int64_t PadstackDef::getPadDiameter(int layerIndex) const {
    auto it = padLayers_.find(layerIndex);
    if (it != padLayers_.end()) {
        return it->second;
    }
    return topDiameter_;
}

} // namespace layout
} // namespace domain
} // namespace fluxcae

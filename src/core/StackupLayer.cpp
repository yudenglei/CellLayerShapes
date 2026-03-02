/**
 * @file StackupLayer.cpp
 * @brief StackupLayer implementation
 */
#include "fluxcae/domain/layout/StackupLayer.h"

namespace fluxcae {
namespace domain {
namespace layout {

StackupLayer::StackupLayer(const std::string& name, LayerType type, Id id)
    : id_(id), name_(name), type_(type), index_(0), isOuter_(false),
      thickness_(35), copperThickness_(35),
      direction_(LayerDirection::Any), defaultWidth_(150),
      defaultSpacing_(150), impedanceTarget_(50),
      color_(0x00FF00), opacity_(255), visible_(true), locked_(false) {
}

StackupLayer::~StackupLayer() = default;

double StackupLayer::thickness() const {
    if (material_) {
        return material_->thickness();
    }
    return thickness_;
}

void StackupLayer::setThickness(double t) {
    thickness_ = t;
}

const char* StackupLayer::typeToString(LayerType type) {
    switch (type) {
        case LayerType::Signal: return "Signal";
        case LayerType::Plane: return "Plane";
        case LayerType::Mixed: return "Mixed";
        case LayerType::SolderMaskTop: return "SolderMaskTop";
        case LayerType::SolderMaskBottom: return "SolderMaskBottom";
        case LayerType::SilkscreenTop: return "SilkscreenTop";
        case LayerType::SilkscreenBottom: return "SilkscreenBottom";
        case LayerType::Core: return "Core";
        case LayerType::Prepreg: return "Prepreg";
        default: return "Unknown";
    }
}

LayerType StackupLayer::stringToType(const std::string& str) {
    if (str == "Signal") return LayerType::Signal;
    if (str == "Plane") return LayerType::Plane;
    if (str == "Mixed") return LayerType::Mixed;
    if (str == "SolderMaskTop") return LayerType::SolderMaskTop;
    if (str == "SolderMaskBottom") return LayerType::SolderMaskBottom;
    if (str == "SilkscreenTop") return LayerType::SilkscreenTop;
    if (str == "SilkscreenBottom") return LayerType::SilkscreenBottom;
    if (str == "Core") return LayerType::Core;
    if (str == "Prepreg") return LayerType::Prepreg;
    return LayerType::Signal;
}

const char* StackupLayer::directionToString(LayerDirection dir) {
    switch (dir) {
        case LayerDirection::Horizontal: return "Horizontal";
        case LayerDirection::Vertical: return "Vertical";
        case LayerDirection::Any: return "Any";
        default: return "Any";
    }
}

LayerDirection StackupLayer::stringToDirection(const std::string& str) {
    if (str == "Horizontal") return LayerDirection::Horizontal;
    if (str == "Vertical") return LayerDirection::Vertical;
    return LayerDirection::Any;
}

std::string StackupLayer::toString() const {
    return "StackupLayer(" + name_ + ", " + typeToString(type_) + ")";
}

} // namespace layout
} // namespace domain
} // namespace fluxcae

/**
 * @file Layer.cpp
 * @brief Layer implementation
 */
#include "fluxcae/domain/layout/Layer.h"
#include <algorithm>

namespace fluxcae {
namespace domain {
namespace layout {

Layer::Layer(int layerNumber, const std::string& name, Id id)
    : id_(id), layerNumber_(layerNumber), name_(name), 
      visible_(true), locked_(false), opacity_(255) {
}

Layer::~Layer() = default;

bool Layer::addShape(ShapePtr shape) {
    if (!shape) return false;
    shapes_.push_back(shape);
    updateBBox();
    return true;
}

bool Layer::removeShape(ShapePtr shape) {
    auto it = std::find(shapes_.begin(), shapes_.end(), shape);
    if (it != shapes_.end()) {
        shapes_.erase(it);
        updateBBox();
        return true;
    }
    return false;
}

void Layer::clearShapes() {
    shapes_.clear();
    bbox_ = math::Box();
}

void Layer::updateBBox() {
    if (shapes_.empty()) {
        bbox_ = math::Box();
        return;
    }
    
    math::Box newBBox = shapes_.front()->bbox();
    for (size_t i = 1; i < shapes_.size(); ++i) {
        newBBox = math::Box::merge(newBBox, shapes_[i]->bbox());
    }
    bbox_ = newBBox;
}

std::string Layer::toString() const {
    return "Layer(" + std::to_string(layerNumber_) + ", " + name_ + ")";
}

} // namespace layout
} // namespace domain
} // namespace fluxcae

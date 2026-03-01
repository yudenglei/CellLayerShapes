/**
 * @file Cell.cpp
 * @brief Cell implementation
 * @author FluxCAE
 * @date 2024
 */
#include "fluxcae/domain/layout/Cell.h"
#include "fluxcae/domain/layout/Shape.h"
#include "fluxcae/domain/layout/Instance.h"
#include "fluxcae/domain/layout/Via.h"
#include "fluxcae/domain/layout/Drill.h"
#include "fluxcae/foundation/utils/Logger.h"

namespace fluxcae {
namespace domain {
namespace layout {

Cell::Cell(const std::string& name, Id id)
    : id_(id), name_(name) {
    LOG_DEBUG("Cell created: {} with id {}", name, id);
}

bool Cell::addShape(ShapePtr shape) {
    if (!shape) return false;
    shapes_.push_back(shape);
    updateBBox();
    return true;
}

bool Cell::removeShape(ShapePtr shape) {
    auto it = std::find(shapes_.begin(), shapes_.end(), shape);
    if (it != shapes_.end()) {
        shapes_.erase(it);
        updateBBox();
        return true;
    }
    return false;
}

bool Cell::addInstance(InstancePtr instance) {
    if (!instance) return false;
    
    instances_.push_back(instance);
    instanceNameIndex_[instance->name()] = instance;
    updateBBox();
    return true;
}

bool Cell::removeInstance(InstancePtr instance) {
    auto it = std::find(instances_.begin(), instances_.end(), instance);
    if (it != instances_.end()) {
        instanceNameIndex_.erase(instance->name());
        instances_.erase(it);
        updateBBox();
        return true;
    }
    return false;
}

InstancePtr Cell::findInstance(const std::string& name) const {
    auto it = instanceNameIndex_.find(name);
    return (it != instanceNameIndex_.end()) ? it->second : nullptr;
}

bool Cell::addVia(ViaPtr via) {
    if (!via) return false;
    vias_.push_back(via);
    updateBBox();
    return true;
}

bool Cell::removeVia(ViaPtr via) {
    auto it = std::find(vias_.begin(), vias_.end(), via);
    if (it != vias_.end()) {
        vias_.erase(it);
        updateBBox();
        return true;
    }
    return false;
}

bool Cell::addDrill(DrillPtr drill) {
    if (!drill) return false;
    drills_.push_back(drill);
    updateBBox();
    return true;
}

bool Cell::removeDrill(DrillPtr drill) {
    auto it = std::find(drills_.begin(), drills_.end(), drill);
    if (it != drills_.end()) {
        drills_.erase(it);
        updateBBox();
        return true;
    }
    return false;
}

void Cell::transform(const math::Transform& transform) {
    // 对所有形状应用变换
    for (auto& shape : shapes_) {
        shape->transform(transform);
    }
    
    // 对所有实例应用变换
    for (auto& instance : instances_) {
        instance->applyTransform(transform);
    }
    
    updateBBox();
}

void Cell::clear() {
    shapes_.clear();
    instances_.clear();
    vias_.clear();
    drills_.clear();
    instanceNameIndex_.clear();
    bbox_ = math::Box();
}

void Cell::updateBBox() {
    math::Box newBBox;
    
    // 合并所有形状的边界框
    for (auto& shape : shapes_) {
        newBBox = newBBox + shape->bbox();
    }
    
    // 合并所有实例的边界框
    for (auto& instance : instances_) {
        newBBox = newBBox + instance->bbox();
    }
    
    // 合并所有通孔的边界框
    for (auto& via : vias_) {
        newBBox = newBBox + via->bbox();
    }
    
    // 合并所有钻孔的边界框
    for (auto& drill : drills_) {
        newBBox = newBBox + drill->bbox();
    }
    
    bbox_ = newBBox;
}

std::string Cell::toString() const {
    std::ostringstream oss;
    oss << "Cell {\n"
        << "  id: " << id_ << "\n"
        << "  name: " << name_ << "\n"
        << "  shapes: " << shapes_.size() << "\n"
        << "  instances: " << instances_.size() << "\n"
        << "  vias: " << vias_.size() << "\n"
        << "  drills: " << drills_.size() << "\n"
        << "  bbox: " << bbox_.toString() << "\n"
        << "}";
    return oss.str();
}

} // namespace layout
} // namespace domain
} // namespace fluxcae

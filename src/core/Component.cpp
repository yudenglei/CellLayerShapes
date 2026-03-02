/**
 * @file Component.cpp
 * @brief Component implementation
 */
#include "fluxcae/domain/layout/Component.h"
#include "fluxcae/domain/layout/Pin.h"

namespace fluxcae {
namespace domain {
namespace layout {

Component::Component(const std::string& name, ComponentType type, Id id)
    : id_(id), name_(name), type_(type), package_(""),
      value_(""), description_(""), manufacturer_(""), partNumber_("") {
}

Component::~Component() = default;

math::Box Component::bbox() const {
    if (packageShapes_.empty()) {
        return math::Box();
    }
    
    math::Box result = packageShapes_.front()->bbox();
    for (size_t i = 1; i < packageShapes_.size(); ++i) {
        result = math::Box::merge(result, packageShapes_[i]->bbox());
    }
    return result;
}

PinPtr Component::createPin(const std::string& name, const math::Point& position) {
    Id pinId = static_cast<Id>(pins_.size() + 1);
    auto pin = std::make_shared<Pin>(name, position, pinId);
    pin->setComponent(shared_from_this());
    pins_.push_back(pin);
    pinNameIndex_[name] = pin;
    return pin;
}

bool Component::addPin(PinPtr pin) {
    if (!pin || pinNameIndex_.count(pin->name())) return false;
    pin->setComponent(shared_from_this());
    pins_.push_back(pin);
    pinNameIndex_[pin->name()] = pin;
    return true;
}

bool Component::removePin(PinPtr pin) {
    auto it = std::find(pins_.begin(), pins_.end(), pin);
    if (it != pins_.end()) {
        pinNameIndex_.erase(pin->name());
        pins_.erase(it);
        return true;
    }
    return false;
}

PinPtr Component::findPin(const std::string& name) const {
    auto it = pinNameIndex_.find(name);
    return (it != pinNameIndex_.end()) ? it->second : nullptr;
}

void Component::addPackageShape(ShapePtr shape) {
    if (shape) {
        packageShapes_.push_back(shape);
    }
}

const char* Component::typeToString(ComponentType type) {
    switch (type) {
        case ComponentType::Generic: return "Generic";
        case ComponentType::IC: return "IC";
        case ComponentType::Resistor: return "Resistor";
        case ComponentType::Capacitor: return "Capacitor";
        case ComponentType::Inductor: return "Inductor";
        case ComponentType::Diode: return "Diode";
        case ComponentType::Transistor: return "Transistor";
        case ComponentType::Connector: return "Connector";
        case ComponentType::Crystal: return "Crystal";
        case ComponentType::LED: return "LED";
        default: return "Unknown";
    }
}

ComponentType Component::stringToType(const std::string& str) {
    if (str == "Generic") return ComponentType::Generic;
    if (str == "IC") return ComponentType::IC;
    if (str == "Resistor") return ComponentType::Resistor;
    if (str == "Capacitor") return ComponentType::Capacitor;
    if (str == "Inductor") return ComponentType::Inductor;
    if (str == "Diode") return ComponentType::Diode;
    if (str == "Transistor") return ComponentType::Transistor;
    if (str == "Connector") return ComponentType::Connector;
    if (str == "Crystal") return ComponentType::Crystal;
    if (str == "LED") return ComponentType::LED;
    return ComponentType::Generic;
}

std::string Component::toString() const {
    return "Component(" + name_ + ", " + typeToString(type_) + ")";
}

} // namespace layout
} // namespace domain
} // namespace fluxcae

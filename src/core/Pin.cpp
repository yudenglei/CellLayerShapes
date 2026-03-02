/**
 * @file Pin.cpp
 * @brief Pin implementation
 */
#include "fluxcae/domain/layout/Pin.h"

namespace fluxcae {
namespace domain {
namespace layout {

Pin::Pin(const std::string& name, const math::Point& position, Id id)
    : id_(id), name_(name), position_(position),
      direction_(PinDirection::Bidirectional), type_(PinType::Signal) {
}

Pin::~Pin() = default;

math::Box Pin::bbox() const {
    int64_t size = 10000; // 10um pin size
    return math::Box(
        position_.x - size/2, position_.y - size/2,
        position_.x + size/2, position_.y + size/2
    );
}

math::Point Pin::absolutePosition() const {
    if (component_) {
        return component_->transform().transform(position_);
    }
    return position_;
}

const char* Pin::directionToString(PinDirection dir) {
    switch (dir) {
        case PinDirection::Input: return "Input";
        case PinDirection::Output: return "Output";
        case PinDirection::Bidirectional: return "Bidirectional";
        case PinDirection::Power: return "Power";
        case PinDirection::Ground: return "Ground";
        case PinDirection::NoConnect: return "NoConnect";
        default: return "Unknown";
    }
}

PinDirection Pin::stringToDirection(const std::string& str) {
    if (str == "Input") return PinDirection::Input;
    if (str == "Output") return PinDirection::Output;
    if (str == "Bidirectional") return PinDirection::Bidirectional;
    if (str == "Power") return PinDirection::Power;
    if (str == "Ground") return PinDirection::Ground;
    if (str == "NoConnect") return PinDirection::NoConnect;
    return PinDirection::Bidirectional;
}

const char* Pin::typeToString(PinType type) {
    switch (type) {
        case PinType::Signal: return "Signal";
        case PinType::Analog: return "Analog";
        case PinType::Digital: return "Digital";
        case PinType::Clock: return "Clock";
        case PinType::Power: return "Power";
        case PinType::Ground: return "Ground";
        case PinType::Reset: return "Reset";
        case PinType::Test: return "Test";
        default: return "Unknown";
    }
}

PinType Pin::stringToType(const std::string& str) {
    if (str == "Signal") return PinType::Signal;
    if (str == "Analog") return PinType::Analog;
    if (str == "Digital") return PinType::Digital;
    if (str == "Clock") return PinType::Clock;
    if (str == "Power") return PinType::Power;
    if (str == "Ground") return PinType::Ground;
    if (str == "Reset") return PinType::Reset;
    if (str == "Test") return PinType::Test;
    return PinType::Signal;
}

std::string Pin::toString() const {
    return "Pin(" + name_ + ", " + directionToString(direction_) + ")";
}

} // namespace layout
} // namespace domain
} // namespace fluxcae

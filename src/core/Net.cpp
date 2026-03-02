/**
 * @file Net.cpp
 * @brief Net implementation
 */
#include "fluxcae/domain/layout/Net.h"
#include <algorithm>

namespace fluxcae {
namespace domain {
namespace layout {

Net::Net(const std::string& name, Id id)
    : id_(id), name_(name), type_(NetType::Signal),
      priority_(0), resistance_(0.0), capacitance_(0.0) {
}

Net::~Net() = default;

bool Net::addPin(PinPtr pin) {
    if (!pin || pinSet_.count(pin)) return false;
    pins_.push_back(pin);
    pinSet_.insert(pin);
    return true;
}

bool Net::removePin(PinPtr pin) {
    auto it = std::find(pins_.begin(), pins_.end(), pin);
    if (it != pins_.end()) {
        pins_.erase(it);
        pinSet_.erase(pin);
        return true;
    }
    return false;
}

bool Net::addVia(ViaPtr via) {
    if (!via || viaSet_.count(via)) return false;
    vias_.push_back(via);
    viaSet_.insert(via);
    return true;
}

bool Net::removeVia(ViaPtr via) {
    auto it = std::find(vias_.begin(), vias_.end(), via);
    if (it != vias_.end()) {
        vias_.erase(it);
        viaSet_.erase(via);
        return true;
    }
    return false;
}

const char* Net::typeToString(NetType type) {
    switch (type) {
        case NetType::Signal: return "Signal";
        case NetType::Power: return "Power";
        case NetType::Ground: return "Ground";
        case NetType::Analog: return "Analog";
        case NetType::Digital: return "Digital";
        case NetType::Differential: return "Differential";
        case NetType::NoConnect: return "NoConnect";
        default: return "Unknown";
    }
}

NetType Net::stringToType(const std::string& str) {
    if (str == "Signal") return NetType::Signal;
    if (str == "Power") return NetType::Power;
    if (str == "Ground") return NetType::Ground;
    if (str == "Analog") return NetType::Analog;
    if (str == "Digital") return NetType::Digital;
    if (str == "Differential") return NetType::Differential;
    if (str == "NoConnect") return NetType::NoConnect;
    return NetType::Signal;
}

std::string Net::toString() const {
    return "Net(" + name_ + ", " + typeToString(type_) + ")";
}

} // namespace layout
} // namespace domain
} // namespace fluxcae

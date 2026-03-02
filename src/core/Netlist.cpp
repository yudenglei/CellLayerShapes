/**
 * @file Netlist.cpp
 * @brief Netlist implementation
 */
#include "fluxcae/domain/layout/Netlist.h"
#include <algorithm>

namespace fluxcae {
namespace domain {
namespace layout {

Netlist::Netlist(Id id) : id_(id) {}

Netlist::~Netlist() = default;

NetPtr Netlist::createNet(const std::string& name) {
    Id netId = static_cast<Id>(nets_.size() + 1);
    auto net = std::make_shared<Net>(name, netId);
    nets_.push_back(net);
    netNameIndex_[name] = net;
    return net;
}

bool Netlist::deleteNet(NetPtr net) {
    auto it = std::find(nets_.begin(), nets_.end(), net);
    if (it != nets_.end()) {
        netNameIndex_.erase(net->name());
        nets_.erase(it);
        return true;
    }
    return false;
}

NetPtr Netlist::findNet(const std::string& name) const {
    auto it = netNameIndex_.find(name);
    return (it != netNameIndex_.end()) ? it->second : nullptr;
}

ComponentPtr Netlist::createComponent(const std::string& name, ComponentType type) {
    Id compId = static_cast<Id>(components_.size() + 1);
    auto comp = std::make_shared<Component>(name, type, compId);
    components_.push_back(comp);
    componentNameIndex_[name] = comp;
    return comp;
}

bool Netlist::deleteComponent(ComponentPtr component) {
    auto it = std::find(components_.begin(), components_.end(), component);
    if (it != components_.end()) {
        componentNameIndex_.erase(component->name());
        components_.erase(it);
        return true;
    }
    return false;
}

ComponentPtr Netlist::findComponent(const std::string& name) const {
    auto it = componentNameIndex_.find(name);
    return (it != componentNameIndex_.end()) ? it->second : nullptr;
}

bool Netlist::connect(NetPtr net, PinPtr pin) {
    if (!net || !pin) return false;
    
    // Disconnect from previous net if any
    auto prevNet = getNetForPin(pin);
    if (prevNet) {
        disconnect(prevNet, pin);
    }
    
    pin->setNet(net);
    net->addPin(pin);
    pinToNetMap_[pin] = net;
    netToPinsMap_[net].push_back(pin);
    return true;
}

bool Netlist::disconnect(NetPtr net, PinPtr pin) {
    if (!net || !pin) return false;
    
    net->removePin(pin);
    pin->setNet(nullptr);
    pinToNetMap_.erase(pin);
    
    auto& pins = netToPinsMap_[net];
    pins.erase(std::remove(pins.begin(), pins.end(), pin), pins.end());
    return true;
}

NetPtr Netlist::getNetForPin(PinPtr pin) const {
    auto it = pinToNetMap_.find(pin);
    return (it != pinToNetMap_.end()) ? it->second : nullptr;
}

std::vector<PinPtr> Netlist::getPinsForNet(NetPtr net) const {
    auto it = netToPinsMap_.find(net);
    return (it != netToPinsMap_.end()) ? it->second : std::vector<PinPtr>();
}

std::vector<PinPtr> Netlist::getPinsForComponent(ComponentPtr component) const {
    std::vector<PinPtr> result;
    for (auto& pin : component->pins()) {
        result.push_back(pin);
    }
    return result;
}

void Netlist::clear() {
    nets_.clear();
    components_.clear();
    netNameIndex_.clear();
    componentNameIndex_.clear();
    pinToNetMap_.clear();
    netToPinsMap_.clear();
}

void Netlist::cleanOrphanNets() {
    std::vector<NetPtr> orphanNets;
    for (auto net : nets_) {
        if (net->isEmpty()) {
            orphanNets.push_back(net);
        }
    }
    for (auto net : orphanNets) {
        deleteNet(net);
    }
}

} // namespace layout
} // namespace domain
} // namespace fluxcae

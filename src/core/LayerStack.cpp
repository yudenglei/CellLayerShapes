/**
 * @file LayerStack.cpp
 * @brief LayerStack implementation
 */
#include "fluxcae/domain/layout/LayerStack.h"
#include <algorithm>
#include <cmath>

namespace fluxcae {
namespace domain {
namespace layout {

LayerStack::LayerStack(Id id) : id_(id), name_("Default") {}

LayerStack::~LayerStack() = default;

bool LayerStack::addLayer(LayerPtr layer) {
    if (!layer) return false;
    layers_.push_back(layer);
    nameIndex_[layer->name()] = layer;
    layer->setIndex(static_cast<int>(layers_.size()) - 1);
    return true;
}

bool LayerStack::removeLayer(LayerPtr layer) {
    auto it = std::find(layers_.begin(), layers_.end(), layer);
    if (it != layers_.end()) {
        nameIndex_.erase(layer->name());
        layers_.erase(it);
        return true;
    }
    return false;
}

LayerPtr LayerStack::getLayer(size_t index) const {
    if (index < layers_.size()) {
        return layers_[index];
    }
    return nullptr;
}

LayerPtr LayerStack::findLayer(const std::string& name) const {
    auto it = nameIndex_.find(name);
    return (it != nameIndex_.end()) ? it->second : nullptr;
}

LayerPtr LayerStack::findLayerByType(LayerType type) const {
    for (auto layer : layers_) {
        if (layer->type() == type) {
            return layer;
        }
    }
    return nullptr;
}

LayerStack::LayerList LayerStack::getSignalLayers() const {
    LayerList result;
    for (auto layer : layers_) {
        if (layer->type() == LayerType::Signal) {
            result.push_back(layer);
        }
    }
    return result;
}

LayerStack::LayerList LayerStack::getPlaneLayers() const {
    LayerList result;
    for (auto layer : layers_) {
        if (layer->type() == LayerType::Plane) {
            result.push_back(layer);
        }
    }
    return result;
}

LayerStack::LayerList LayerStack::getOuterLayers() const {
    LayerList result;
    for (auto layer : layers_) {
        if (layer->isOuter()) {
            result.push_back(layer);
        }
    }
    return result;
}

LayerStack::LayerList LayerStack::getInnerLayers() const {
    LayerList result;
    for (auto layer : layers_) {
        if (!layer->isOuter()) {
            result.push_back(layer);
        }
    }
    return result;
}

double LayerStack::totalThickness() const {
    double total = 0.0;
    for (auto layer : layers_) {
        total += layer->thickness();
    }
    return total;
}

double LayerStack::thicknessToLayer(size_t toLayerIndex) const {
    double total = 0.0;
    for (size_t i = 0; i < toLayerIndex && i < layers_.size(); ++i) {
        total += layers_[i]->thickness();
    }
    return total;
}

std::shared_ptr<LayerStack> LayerStack::create2LayerBoard() {
    auto stack = std::make_shared<LayerStack>(1);
    stack->setName("2-Layer Board");
    
    // Top signal
    auto topSignal = std::make_shared<StackupLayer>("TopSignal", LayerType::Signal, 1);
    topSignal->setThickness(35); // 35um copper
    topSignal->setOuter(true);
    
    // Core
    auto core = std::make_shared<StackupLayer>("Core", LayerType::Core, 2);
    core->setThickness(1500); // 1.5mm core
    
    // Bottom signal
    auto bottomSignal = std::make_shared<StackupLayer>("BottomSignal", LayerType::Signal, 3);
    bottomSignal->setThickness(35);
    bottomSignal->setOuter(true);
    
    stack->addLayer(topSignal);
    stack->addLayer(core);
    stack->addLayer(bottomSignal);
    
    return stack;
}

std::shared_ptr<LayerStack> LayerStack::create4LayerBoard() {
    auto stack = std::make_shared<LayerStack>(1);
    stack->setName("4-Layer Board");
    
    auto topSignal = std::make_shared<StackupLayer>("TopSignal", LayerType::Signal, 1);
    topSignal->setThickness(35);
    topSignal->setOuter(true);
    
    auto prepreg1 = std::make_shared<StackupLayer>("Prepreg1", LayerType::Prepreg, 2);
    prepreg1->setThickness(200);
    
    auto innerPlane = std::make_shared<StackupLayer>("GNDPlane", LayerType::Plane, 3);
    innerPlane->setThickness(35);
    
    auto core = std::make_shared<StackupLayer>("Core", LayerType::Core, 4);
    core->setThickness(800);
    
    auto innerPlane2 = std::make_shared<StackupLayer>("PowerPlane", LayerType::Plane, 5);
    innerPlane2->setThickness(35);
    
    auto prepreg2 = std::make_shared<StackupLayer>("Prepreg2", LayerType::Prepreg, 6);
    prepreg2->setThickness(200);
    
    auto bottomSignal = std::make_shared<StackupLayer>("BottomSignal", LayerType::Signal, 7);
    bottomSignal->setThickness(35);
    bottomSignal->setOuter(true);
    
    stack->addLayer(topSignal);
    stack->addLayer(prepreg1);
    stack->addLayer(innerPlane);
    stack->addLayer(core);
    stack->addLayer(innerPlane2);
    stack->addLayer(prepreg2);
    stack->addLayer(bottomSignal);
    
    return stack;
}

std::shared_ptr<LayerStack> LayerStack::create6LayerBoard() {
    auto stack = std::make_shared<LayerStack>(1);
    stack->setName("6-Layer Board");
    
    // Simplified 6-layer stackup
    for (int i = 0; i < 6; ++i) {
        auto layer = std::make_shared<StackupLayer>(
            "L" + std::to_string(i + 1), 
            (i == 0 || i == 5) ? LayerType::Signal : LayerType::Mixed, 
            static_cast<Id>(i + 1)
        );
        layer->setThickness(35);
        layer->setOuter(i == 0 || i == 5);
        stack->addLayer(layer);
    }
    
    return stack;
}

std::string LayerStack::toString() const {
    return "LayerStack(" + name_ + ", " + std::to_string(layers_.size()) + " layers)";
}

} // namespace layout
} // namespace domain
} // namespace fluxcae

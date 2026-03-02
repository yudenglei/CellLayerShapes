/**
 * @file Component.h
 * @brief Component - Electronic component
 * @author FluxCAE
 * @date 2024
 */
#pragma once

#include "fluxcae/domain/layout/Pin.h"
#include "fluxcae/domain/layout/Shape.h"
#include "fluxcae/foundation/math/Transform.h"
#include "fluxcae/foundation/math/Box.h"
#include <string>
#include <vector>
#include <memory>
#include <unordered_map>

namespace fluxcae {
namespace domain {
namespace layout {

enum class ComponentType {
    Generic,
    IC,
    Resistor,
    Capacitor,
    Inductor,
    Diode,
    Transistor,
    Connector,
    Crystal,
    LED,
    Switch,
    Fuse,
    Relay,
    Transformer,
    Sensor,
    Motor,
    Battery,
    CrystalOscillator,
    Module
};

class Component : public std::enable_shared_from_this<Component> {
public:
    using Id = uint64_t;
    using PinPtr = std::shared_ptr<Pin>;
    using ShapePtr = std::shared_ptr<Shape>;
    using PinList = std::vector<PinPtr>;

    Component(const std::string& name, ComponentType type = ComponentType::Generic, 
              Id id = 0);

    ~Component() = default;

    Id id() const { return id_; }
    void setId(Id id) { id_ = id; }

    const std::string& name() const { return name_; }
    void setName(const std::string& name) { name_ = name; }

    ComponentType type() const { return type_; }
    void setType(ComponentType type) { type_ = type; }

    const std::string& package() const { return package_; }
    void setPackage(const std::string& pkg) { package_ = pkg; }

    const math::Transform& transform() const { return transform_; }
    math::Transform& transform() { return transform_; }

    math::Box bbox() const;

    PinPtr createPin(const std::string& name, const math::Point& position);
    bool addPin(PinPtr pin);
    bool removePin(PinPtr pin);
    const PinList& pins() const { return pins_; }
    size_t pinCount() const { return pins_.size(); }
    PinPtr findPin(const std::string& name) const;

    void addPackageShape(ShapePtr shape);
    const std::vector<ShapePtr>& packageShapes() const { return packageShapes_; }

    const std::string& value() const { return value_; }
    void setValue(const std::string& val) { value_ = val; }

    const std::string& description() const { return description_; }
    void setDescription(const std::string& desc) { description_ = desc; }

    const std::string& manufacturer() const { return manufacturer_; }
    void setManufacturer(const std::string& mfr) { manufacturer_ = mfr; }

    const std::string& partNumber() const { return partNumber_; }
    void setPartNumber(const std::string& pn) { partNumber_ = pn; }

    static const char* typeToString(ComponentType type);
    static ComponentType stringToType(const std::string& str);
    std::string toString() const;

private:
    Id id_;
    std::string name_;
    ComponentType type_;
    std::string package_;
    math::Transform transform_;
    
    PinList pins_;
    std::unordered_map<std::string, PinPtr> pinNameIndex_;
    
    std::vector<ShapePtr> packageShapes_;
    
    std::string value_;
    std::string description_;
    std::string manufacturer_;
    std::string partNumber_;
};

} // namespace layout
} // namespace domain
} // namespace fluxcae

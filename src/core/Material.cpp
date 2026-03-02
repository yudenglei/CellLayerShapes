/**
 * @file Material.cpp
 * @brief Material implementation
 */
#include "fluxcae/domain/layout/Material.h"

namespace fluxcae {
namespace domain {
namespace layout {

Material::Material(const std::string& name, MaterialType type, Id id)
    : id_(id), name_(name), type_(type),
      dielectricConstant_(4.5), lossTangent_(0.02),
      volumeResistivity_(1e14), dielectricStrength_(20e6),
      thermalConductivity_(0.3), thermalExpansionCoeff_(14e-6),
      glassTransitionTemp_(130), maxOperatingTemp_(125),
      density_(1850), youngsModulus_(24e9), tensileStrength_(60e6),
      thickness_(1.6e-3), color_("green") {
}

Material::~Material() = default;

Material Material::createFR4() {
    Material mat("FR4", MaterialType::FR4, 1);
    mat.setDielectricConstant(4.5);
    mat.setLossTangent(0.02);
    mat.setDensity(1850);
    mat.setGlassTransitionTemp(130);
    mat.setThickness(1.6e-3);
    mat.setColor("green");
    return mat;
}

Material Material::createCopper() {
    Material mat("Copper", MaterialType::Conductor, 2);
    mat.setVolumeResistivity(1.68e-8);
    mat.setThermalConductivity(401);
    mat.setDensity(8960);
    mat.setThickness(35e-6); // 1oz copper
    mat.setColor("copper");
    return mat;
}

Material Material::createRogers() {
    Material mat("Rogers RT/duroid", MaterialType::Rogers, 3);
    mat.setDielectricConstant(2.2);
    mat.setLossTangent(0.0009);
    mat.setDensity(2200);
    mat.setMaxOperatingTemp(260);
    mat.setThickness(0.508e-3);
    mat.setColor("white");
    return mat;
}

const char* Material::typeToString(MaterialType type) {
    switch (type) {
        case MaterialType::Dielectric: return "Dielectric";
        case MaterialType::Conductor: return "Conductor";
        case MaterialType::SolderMask: return "SolderMask";
        case MaterialType::Silkscreen: return "Silkscreen";
        case MaterialType::Core: return "Core";
        case MaterialType::Prepreg: return "Prepreg";
        case MaterialType::FR4: return "FR4";
        case MaterialType::Rogers: return "Rogers";
        case MaterialType::PTFE: return "PTFE";
        default: return "Unknown";
    }
}

MaterialType Material::stringToType(const std::string& str) {
    if (str == "Dielectric") return MaterialType::Dielectric;
    if (str == "Conductor") return MaterialType::Conductor;
    if (str == "SolderMask") return MaterialType::SolderMask;
    if (str == "Silkscreen") return MaterialType::Silkscreen;
    if (str == "Core") return MaterialType::Core;
    if (str == "Prepreg") return MaterialType::Prepreg;
    if (str == "FR4") return MaterialType::FR4;
    if (str == "Rogers") return MaterialType::Rogers;
    if (str == "PTFE") return MaterialType::PTFE;
    return MaterialType::Dielectric;
}

std::string Material::toString() const {
    return "Material(" + name_ + ", dk=" + std::to_string(dielectricConstant_) + ")";
}

} // namespace layout
} // namespace domain
} // namespace fluxcae

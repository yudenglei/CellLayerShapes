#ifndef FLUXCAE_LAYOUT_H
#define FLUXCAE_LAYOUT_H

#include "fluxcae/domain/layout/Layer.h"
#include "fluxcae/domain/layout/Cell.h"
#include "fluxcae/domain/layout/Via.h"
#include "fluxcae/domain/layout/Drill.h"
#include "fluxcae/domain/netlist/Netlist.h"
#include "fluxcae/domain/stackup/LayerStack.h"
#include <memory>
#include <map>
#include <string>
#include <vector>

namespace fluxcae {

/// @brief ID类型定义
using LayerID = uint32_t;
using CellID = uint64_t;
using ShapeID = uint64_t;
using ViaID = uint64_t;
using DrillID = uint64_t;
using ComponentID = uint64_t;
using NetID = uint64_t;
using PinID = uint64_t;

/// @brief 布局数据主类
/// @details 整个设计文件的主入口，包含所有层、Cell、网表等信息
class Layout {
public:
    using LayerMap = std::map<LayerID, std::unique_ptr<Layer>>;
    using CellMap = std::map<CellID, std::unique_ptr<Cell>>;
    
private:
    // 元数据
    std::string version_;
    std::string creator_;
    std::string filename_;
    
    // Layer管理
    LayerMap layers_;
    std::map<int, LayerID> gds_to_layer_;  // GDS layer/datatype -> LayerID
    LayerID next_layer_id_ = 1;
    
    // Cell树
    Cell* root_cell_;
    CellMap cells_;
    std::map<std::string, CellID> name_to_cell_;
    CellID next_cell_id_ = 1;
    
    // 跨层对象注册表
    ViaRegistry via_registry_;
    DrillRegistry drill_registry_;
    ComponentRegistry component_registry_;
    
    // 网表
    Netlist netlist_;
    
    // 层叠结构
    std::unique_ptr<LayerStack> layer_stack_;
    
public:
    Layout();
    ~Layout();
    
    // 删除拷贝，保留移动
    Layout(const Layout&) = delete;
    Layout& operator=(const Layout&) = delete;
    Layout(Layout&&) noexcept;
    Layout& operator=(Layout&&) noexcept;
    
    // ===== 元数据 =====
    void setVersion(const std::string& v) { version_ = v; }
    const std::string& version() const { return version_; }
    void setCreator(const std::string& c) { creator_ = c; }
    const std::string& creator() const { return creator_; }
    void setFilename(const std::string& f) { filename_ = f; }
    const std::string& filename() const { return filename_; }
    
    // ===== Layer操作 =====
    LayerID createLayer(int gds_layer, int gds_datatype);
    Layer* getLayer(LayerID id);
    const Layer* getLayer(LayerID id) const;
    Layer* getLayerByGDS(int gds_layer, int gds_datatype);
    void removeLayer(LayerID id);
    
    LayerMap& layers() { return layers_; }
    const LayerMap& layers() const { return layers_; }
    
    size_t layerCount() const { return layers_.size(); }
    
    // ===== Cell操作 =====
    CellID createCell(const std::string& name);
    Cell* getCell(CellID id);
    const Cell* getCell(CellID id) const;
    Cell* getCellByName(const std::string& name);
    void deleteCell(CellID id);
    
    Cell* rootCell() { return root_cell_; }
    const Cell* rootCell() const { return root_cell_; }
    
    CellMap& cells() { return cells_; }
    const CellMap& cells() const { return cells_; }
    
    size_t cellCount() const { return cells_.size(); }
    
    // ===== Via操作 =====
    ViaID createVia(const Via& via);
    Via* getVia(ViaID id);
    const Via* getVia(ViaID id) const;
    void removeVia(ViaID id);
    
    ViaRegistry& viaRegistry() { return via_registry_; }
    const ViaRegistry& viaRegistry() const { return via_registry_; }
    
    size_t viaCount() const { return via_registry_.size(); }
    
    // ===== Drill操作 =====
    DrillID createDrill(const Drill& drill);
    Drill* getDrill(DrillID id);
    const Drill* getDrill(DrillID id) const;
    void removeDrill(DrillID id);
    
    DrillRegistry& drillRegistry() { return drill_registry_; }
    const DrillRegistry& drillRegistry() const { return drill_registry_; }
    
    size_t drillCount() const { return drill_registry_.size(); }
    
    // ===== Component操作 =====
    ComponentID createComponent(const Component& comp);
    Component* getComponent(ComponentID id);
    const Component* getComponent(ComponentID id) const;
    void removeComponent(ComponentID id);
    
    ComponentRegistry& componentRegistry() { return component_registry_; }
    const ComponentRegistry& componentRegistry() const { return component_registry_; }
    
    size_t componentCount() const { return component_registry_.size(); }
    
    // ===== Netlist操作 =====
    Netlist& netlist() { return netlist_; }
    const Netlist& netlist() const { return netlist_; }
    
    // ===== LayerStack操作 =====
    void setLayerStack(std::unique_ptr<LayerStack> stack);
    LayerStack* layerStack() { return layer_stack_.get(); }
    const LayerStack* layerStack() const { return layer_stack_.get(); }
    
    bool hasLayerStack() const { return layer_stack_ != nullptr; }
    
    // ===== 序列化 =====
    void save(const std::string& path) const;
    void load(const std::string& path);
    
    // ===== 统计信息 =====
    struct Stats {
        size_t num_layers = 0;
        size_t num_cells = 0;
        size_t num_shapes = 0;
        size_t num_vias = 0;
        size_t num_drills = 0;
        size_t num_components = 0;
        size_t num_nets = 0;
        size_t memory_estimate() const;
    };
    
    Stats statistics() const;
    
    // ===== 验证 =====
    bool validate() const;
    std::vector<std::string> getValidationErrors() const;
};

} // namespace fluxcae

#endif // FLUXCAE_LAYOUT_H

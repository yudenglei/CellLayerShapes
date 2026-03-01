#ifndef FLUXCAE_VIA_H
#define FLUXCAE_VIA_H

#include "fluxcae/foundation/math/Point.h"
#include "fluxcae/foundation/math/Box.h"
#include <string>
#include <vector>
#include <memory>

namespace fluxcae {

/// @brief Via类型
enum class ViaType {
    Through,      // 通孔（贯穿所有层）
    Blind,         // 盲孔（表层到内层）
    Buried,        // 埋孔（内层到内层）
    Micro          // 微孔（HDI用）
};

/// @brief Padstack定义
class PadstackDef {
public:
    using PadShape = std::unique_ptr<class Shape>;
    
private:
    uint64_t id_;
    std::string name_;
    
    // 钻孔直径
    int drill_diameter_;
    
    // 各层焊盘尺寸（层索引 -> 焊盘尺寸）
    // -1表示该层无焊盘
    std::map<int, int> pad_diameters_;
    std::map<int, int> anti_pad_diameters_;
    
public:
    PadstackDef(uint64_t id, const std::string& name, int drill_diameter);
    
    uint64_t id() const { return id_; }
    const std::string& name() const { return name_; }
    int drillDiameter() const { return drill_diameter_; }
    
    void setPadDiameter(int layer, int diameter);
    int getPadDiameter(int layer) const;
    
    void setAntiPadDiameter(int layer, int diameter);
    int getAntiPadDiameter(int layer) const;
    
    Box getPadBox(int layer, const Point& position) const;
    Box getDrillBox(const Point& position) const;
    
    std::vector<int> getAffectedLayers(int start_layer, int end_layer) const;
};

/// @brief 过孔对象
class Via {
private:
    ViaID id_;
    std::string name_;
    
    // 位置
    Point position_;
    
    // 层范围
    int start_layer_;
    int end_layer_;
    
    // Padstack定义
    uint64_t padstack_def_id_;
    const PadstackDef* padstack_def_;
    
    // 所属网络
    NetID net_id_;
    
    // Via类型
    ViaType type_;
    
    // 制造属性
    bool is_plated_;           // 是否金属化
    bool is_backdrilled_;      // 是否背钻
    int backdrill_depth_;      // 背钻深度
    
    // 缓存的包围盒
    Box bounding_box_;
    
public:
    Via(ViaID id, const Point& position, int start_layer, int end_layer);
    ~Via();
    
    // ===== 基本属性 =====
    ViaID id() const { return id_; }
    const std::string& name() const { return name_; }
    void setName(const std::string& name) { name_ = name; }
    
    const Point& position() const { return position_; }
    void setPosition(const Point& pos) { position_ = pos; updateBoundingBox(); }
    
    int startLayer() const { return start_layer_; }
    int endLayer() const { return end_layer_; }
    void setLayerRange(int start, int end) { start_layer_ = start; end_layer_ = end; updateBoundingBox(); }
    
    ViaType type() const { return type_; }
    void setType(ViaType t) { type_ = t; }
    
    // ===== Padstack =====
    uint64_t padstackDefId() const { return padstack_def_id_; }
    void setPadstackDef(uint64_t id, const PadstackDef* def);
    const PadstackDef* padstackDef() const { return padstack_def_; }
    
    // ===== 网络 =====
    NetID netId() const { return net_id_; }
    void setNetId(NetID id) { net_id_ = id; }
    
    // ===== 制造属性 =====
    bool isPlated() const { return is_plated_; }
    void setPlated(bool v) { is_plated_ = v; }
    
    bool isBackdrilled() const { return is_backdrilled_; }
    void setBackdrilled(bool v) { is_backdrilled_ = v; }
    
    int backdrillDepth() const { return backdrill_depth_; }
    void setBackdrillDepth(int d) { backdrill_depth_ = d; }
    
    // ===== 几何查询 =====
    const Box& boundingBox() const { return bounding_box_; }
    
    Box getPadBox(int layer) const;
    Box getDrillBox() const;
    
    // 获取涉及的层列表
    std::vector<int> getAffectedLayers() const;
    
    // 判断是否跨越指定层
    bool spansLayer(int layer) const {
        return layer >= start_layer_ && layer <= end_layer_;
    }
    
    // 判断是否是通孔
    bool isThroughVia() const { return type_ == ViaType::Through; }
    
private:
    void updateBoundingBox();
};

/// @brief Via注册表
class ViaRegistry {
private:
    std::map<ViaID, std::unique_ptr<Via>> vias_;
    std::map<NetID, std::vector<ViaID>> net_to_vias_;  // NetID -> ViaIDs
    
public:
    ViaID add(std::unique_ptr<Via> via);
    void remove(ViaID id);
    Via* get(ViaID id);
    const Via* get(ViaID id) const;
    
    std::map<ViaID, std::unique_ptr<Via>>& all() { return vias_; }
    const std::map<ViaID, std::unique_ptr<Via>>& all() const { return vias_; }
    
    size_t size() const { return vias_.size(); }
    void clear();
    
    std::vector<ViaID> getViasByNet(NetID net_id) const;
    
    void updateNetIndex(ViaID id, NetID old_net, NetID new_net);
};

} // namespace fluxcae

#endif // FLUXCAE_VIA_H

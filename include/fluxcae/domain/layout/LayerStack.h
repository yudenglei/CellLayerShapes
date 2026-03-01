/**
 * @file LayerStack.h
 * @brief LayerStack - PCB层叠结构管理
 * @author FluxCAE
 * @date 2024
 */
#pragma once

#include "fluxcae/domain/layout/StackupLayer.h"
#include "fluxcae/domain/layout/Material.h"
#include <string>
#include <vector>
#include <memory>
#include <unordered_map>

namespace fluxcae {
namespace domain {
namespace layout {

/**
 * @brief LayerStack 表示PCB层叠结构
 * 
 * 管理PCB的多层结构，包括：
 * - 信号层
 * - 电源层
 * - 丝印层
 * - 阻焊层
 * - 钻孔层
 */
class LayerStack {
public:
    using Id = uint64_t;
    using LayerPtr = std::shared_ptr<StackupLayer>;
    using LayerList = std::vector<LayerPtr>;

    /**
     * @brief 构造函数
     * @param id 层叠结构唯一标识符
     */
    explicit LayerStack(Id id = 0);

    /**
     * @brief 析构函数
     */
    ~LayerStack() = default;

    // ===== 属性访问 =====

    /// 获取层叠ID
    Id id() const { return id_; }
    
    /// 设置层叠ID
    void setId(Id id) { id_ = id; }

    /// 获取层叠名称
    const std::string& name() const { return name_; }
    
    /// 设置层叠名称
    void setName(const std::string& n) { name_ = n; }

    // ===== 层管理 =====

    /// 获取层总数
    size_t layerCount() const { return layers_.size(); }

    /**
     * @brief 添加层
     * @param layer 层指针
     * @return 添加成功返回true
     */
    bool addLayer(LayerPtr layer);

    /**
     * @brief 移除层
     * @param layer 层指针
     * @return 移除成功返回true
     */
    bool removeLayer(LayerPtr layer);

    /**
     * @brief 根据索引获取层
     * @param index 层索引（从0开始）
     * @return 层指针，无效索引返回nullptr
     */
    LayerPtr getLayer(size_t index) const;

    /**
     * @brief 根据名称查找层
     * @param name 层名称
     * @return 层指针，未找到返回nullptr
     */
    LayerPtr findLayer(const std::string& name) const;

    /**
     * @brief 根据类型查找层
     * @param type 层类型
     * @return 层指针，未找到返回nullptr
     */
    LayerPtr findLayerByType(LayerType type) const;

    /// 获取所有层
    const LayerList& layers() const { return layers_; }

    // ===== 信号层管理 =====

    /**
     * @brief 获取所有信号层
     * @return 信号层列表
     */
    LayerList getSignalLayers() const;

    /**
     * @brief 获取所有平面层
     * @return 平面层列表
     */
    LayerList getPlaneLayers() const;

    /**
     * @brief 获取所有外层
     * @return 外层列表
     */
    LayerList getOuterLayers() const;

    /**
     * @brief 获取所有内层
     * @return 内层列表
     */
    LayerList getInnerLayers() const;

    // ===== 厚度计算 =====

    /// 获取总板厚（单位：微米）
    double totalThickness() const;

    /// 获取从顶层到指定层的厚度
    double thicknessToLayer(size_t toLayerIndex) const;

    // ===== 预定义层叠 =====

    /**
     * @brief 创建标准2层板
     * @return 层叠指针
     */
    static std::shared_ptr<LayerStack> create2LayerBoard();

    /**
     * @brief 创建标准4层板
     * @return 层叠指针
     */
    static std::shared_ptr<LayerStack> create4LayerBoard();

    /**
     * @brief 创建标准6层板
     * @return 层叠指针
     */
    static std::shared_ptr<LayerStack> create6LayerBoard();

    // ===== 序列化 =====

    /**
     * @brief 导出为字符串表示
     * @return 字符串表示
     */
    std::string toString() const;

private:
    Id id_;
    std::string name_;
    LayerList layers_;
    
    // 名称索引
    std::unordered_map<std::string, LayerPtr> nameIndex_;
};

} // namespace layout
} // namespace domain
} // namespace fluxcae

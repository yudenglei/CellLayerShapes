/**
 * @file Layout.cpp
 * @brief Layout implementation
 * @author FluxCAE
 * @date 2024
 */
#include "fluxcae/domain/layout/Layout.h"
#include "fluxcae/foundation/utils/Logger.h"
#include <algorithm>

namespace fluxcae {
namespace domain {
namespace layout {

Layout::Layout()
    : id_(0), databaseUnit_(0.001), userUnit_(1.0) {
    LOG_INFO("Layout created");
}

Layout::~Layout() {
    LOG_INFO("Layout destroyed");
}

Layout::Id Layout::createCell(const std::string& name) {
    Id cellId = cellIdGenerator_.next();
    auto cell = std::make_shared<Cell>(name, cellId);
    cells_.push_back(cell);
    cellNameIndex_[name] = cell;
    cellIdIndex_[cellId] = cell;
    
    LOG_DEBUG("Created cell: {} with id {}", name, cellId);
    return cellId;
}

bool Layout::deleteCell(Id cellId) {
    auto it = cellIdIndex_.find(cellId);
    if (it == cellIdIndex_.end()) {
        return false;
    }
    
    auto cell = it->second;
    cellNameIndex_.erase(cell->name());
    cells_.erase(std::remove(cells_.begin(), cells_.end(), cell), cells_.end());
    cellIdIndex_.erase(it);
    
    LOG_DEBUG("Deleted cell with id {}", cellId);
    return true;
}

CellPtr Layout::findCell(const std::string& name) const {
    auto it = cellNameIndex_.find(name);
    return (it != cellNameIndex_.end()) ? it->second : nullptr;
}

CellPtr Layout::findCell(Id cellId) const {
    auto it = cellIdIndex_.find(cellId);
    return (it != cellIdIndex_.end()) ? it->second : nullptr;
}

Layout::Id Layout::createLayer(int layerNumber, const std::string& layerName) {
    Id layerId = layerIdGenerator_.next();
    auto layer = std::make_shared<Layer>(layerNumber, layerName, layerId);
    layers_.push_back(layer);
    layerNumberIndex_[layerNumber] = layer;
    layerNameIndex_[layerName] = layer;
    
    LOG_DEBUG("Created layer: {} ({}) with id {}", layerName, layerNumber, layerId);
    return layerId;
}

bool Layout::deleteLayer(Id layerId) {
    auto it = std::find_if(layers_.begin(), layers_.end(),
        [layerId](const LayerPtr& layer) { return layer->id() == layerId; });
    
    if (it == layers_.end()) {
        return false;
    }
    
    auto layer = *it;
    layerNumberIndex_.erase(layer->layerNumber());
    layerNameIndex_.erase(layer->name());
    layers_.erase(it);
    
    LOG_DEBUG("Deleted layer with id {}", layerId);
    return true;
}

LayerPtr Layout::findLayer(int layerNumber) const {
    auto it = layerNumberIndex_.find(layerNumber);
    return (it != layerNumberIndex_.end()) ? it->second : nullptr;
}

LayerPtr Layout::findLayer(const std::string& layerName) const {
    auto it = layerNameIndex_.find(layerName);
    return (it != layerNameIndex_.end()) ? it->second : nullptr;
}

void Layout::setTopCell(Id cellId) {
    topCell_ = findCell(cellId);
}

void Layout::setTopCell(const std::string& name) {
    topCell_ = findCell(name);
}

void Layout::clear() {
    cells_.clear();
    layers_.clear();
    cellNameIndex_.clear();
    cellIdIndex_.clear();
    layerNumberIndex_.clear();
    layerNameIndex_.clear();
    topCell_ = nullptr;
    
    LOG_INFO("Layout cleared");
}

std::string Layout::toString() const {
    std::ostringstream oss;
    oss << "Layout {\n"
        << "  cells: " << cells_.size() << "\n"
        << "  layers: " << layers_.size() << "\n"
        << "  databaseUnit: " << databaseUnit_ << "\n"
        << "  userUnit: " << userUnit_ << "\n";
    
    if (topCell_) {
        oss << "  topCell: " << topCell_->name() << "\n";
    }
    
    oss << "}";
    return oss.str();
}

} // namespace layout
} // namespace domain
} // namespace fluxcae

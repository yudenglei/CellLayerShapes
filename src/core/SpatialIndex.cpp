/**
 * @file SpatialIndex.cpp
 * @brief SpatialIndex implementation
 */
#include "fluxcae/domain/layout/SpatialIndex.h"
#include <algorithm>
#include <cmath>

namespace fluxcae {
namespace domain {
namespace layout {

// RTree implementation
RTree::RTree() : root_(nullptr) {}

RTree::~RTree() {
    delete root_;
}

void RTree::insert(const math::Box& box, void* data) {
    if (!root_) {
        root_ = new RTreeNode(true);
    }
    root_->insert(box, data);
}

bool RTree::remove(const math::Box& box, void* data) {
    if (!root_) return false;
    return root_->remove(box, data);
}

void RTree::query(const math::Box& box, std::vector<void*>& results) const {
    if (!root_) return;
    root_->query(box, results);
}

void RTree::queryPoint(const math::Point& point, std::vector<void*>& results) const {
    math::Box box(point.x, point.y, point.x, point.y);
    query(box, results);
}

void RTree::clear() {
    delete root_;
    root_ = nullptr;
}

// RTreeNode
RTreeNode::RTreeNode(bool isLeaf) : isLeaf_(isLeaf), parent_(nullptr) {}

RTreeNode::~RTreeNode() {
    for (auto child : children_) {
        delete child;
    }
}

void RTreeNode::insert(const math::Box& box, void* data) {
    if (isLeaf_) {
        entries_.push_back({box, data});
        // Simplified: no splitting for now
    } else {
        // Find best child
        RTreeNode* best = children_.front();
        int64_t bestArea = best->bbox_.area();
        for (auto child : children_) {
            math::Box merged = math::Box::merge(child->bbox_, box);
            if (merged.area() < bestArea) {
                best = child;
                bestArea = merged.area();
            }
        }
        best->insert(box, data);
        bbox_ = math::Box::merge(bbox_, box);
    }
}

bool RTreeNode::remove(const math::Box& box, void* data) {
    if (isLeaf_) {
        for (auto it = entries_.begin(); it != entries_.end(); ++it) {
            if (it->box == box && it->data == data) {
                entries_.erase(it);
                return true;
            }
        }
        return false;
    } else {
        for (auto child : children_) {
            if (child->bbox_.intersects(box)) {
                if (child->remove(box, data)) {
                    // Update bbox
                    recalcBBox();
                    return true;
                }
            }
        }
        return false;
    }
}

void RTreeNode::query(const math::Box& box, std::vector<void*>& results) const {
    if (isLeaf_) {
        for (const auto& entry : entries_) {
            if (entry.box.intersects(box)) {
                results.push_back(entry.data);
            }
        }
    } else {
        for (auto child : children_) {
            if (child->bbox_.intersects(box)) {
                child->query(box, results);
            }
        }
    }
}

void RTreeNode::recalcBBox() {
    if (children_.empty() && entries_.empty()) {
        bbox_ = math::Box();
        return;
    }
    
    if (isLeaf_) {
        bbox_ = entries_.front().box;
        for (size_t i = 1; i < entries_.size(); ++i) {
            bbox_ = math::Box::merge(bbox_, entries_[i].box);
        }
    } else {
        bbox_ = children_.front()->bbox_;
        for (size_t i = 1; i < children_.size(); ++i) {
            bbox_ = math::Box::merge(bbox_, children_[i]->bbox_);
        }
    }
}

// GridIndex
GridIndex::GridIndex(int64_t cellSize) : cellSize_(cellSize) {}

GridIndex::~GridIndex() = default;

void GridIndex::insert(const math::Box& box, void* data) {
    int64_t minX = box.x1 / cellSize_;
    int64_t maxX = box.x2 / cellSize_;
    int64_t minY = box.y1 / cellSize_;
    int64_t maxY = box.y2 / cellSize_;
    
    for (int64_t x = minX; x <= maxX; ++x) {
        for (int64_t y = minY; y <= maxY; ++y) {
            grid_[{x, y}].push_back(data);
        }
    }
}

void GridIndex::query(const math::Box& box, std::vector<void*>& results) const {
    int64_t minX = box.x1 / cellSize_;
    int64_t maxX = box.x2 / cellSize_;
    int64_t minY = box.y1 / cellSize_;
    int64_t maxY = box.y2 / cellSize_;
    
    std::set<void*> resultSet;
    
    for (int64_t x = minX; x <= maxX; ++x) {
        for (int64_t y = minY; y <= maxY; ++y) {
            auto it = grid_.find({x, y});
            if (it != grid_.end()) {
                for (auto data : it->second) {
                    resultSet.insert(data);
                }
            }
        }
    }
    
    results.assign(resultSet.begin(), resultSet.end());
}

void GridIndex::clear() {
    grid_.clear();
}

} // namespace layout
} // namespace domain
} // namespace fluxcae

#ifndef FLUXCAE_SPATIAL_INDEX_H
#define FLUXCAE_SPATIAL_INDEX_H

#include "fluxcae/foundation/math/Box.h"
#include <vector>
#include <memory>
#include <algorithm>
#include <stack>

namespace fluxcae {

/// @brief 空间索引条目
struct SpatialEntry {
    uint64_t id;
    Box bounds;
    
    SpatialEntry() = default;
    SpatialEntry(uint64_t i, const Box& b) : id(i), bounds(b) {}
};

/// @brief 简单R-Tree实现
/// @details 用于空间查询的高效索引结构
class SpatialIndex {
public:
    struct Node {
        Box bounds;
        bool is_leaf = true;
        std::vector<SpatialEntry> entries;
        std::vector<std::unique_ptr<Node>> children;
        Node* parent = nullptr;
        
        Node(bool leaf = true) : is_leaf(leaf) {}
    };

private:
    static const size_t MAX_ENTRIES = 4;
    static const size_t MIN_ENTRIES = 2;
    
    std::unique_ptr<Node> root_;
    size_t size_ = 0;
    
    // 辅助方法
    Box calculateBounds(const std::vector<SpatialEntry>& entries) const;
    Box calculateBounds(const std::vector<Node*>& children) const;
    
    void splitNode(Node* node);
    void rebalance(Node* node);
    void removeEntry(Node* node, uint64_t id);
    void updateBounds(Node* node);
    
    // 查询辅助
    void queryRecursive(const Node* node, const Box& region, 
                       std::vector<uint64_t>& result) const;
    
public:
    SpatialIndex();
    ~SpatialIndex();
    
    // 禁止拷贝
    SpatialIndex(const SpatialIndex&) = delete;
    SpatialIndex& operator=(const SpatialIndex&) = delete;
    
    // 插入
    void insert(uint64_t id, const Box& bounds);
    
    // 删除
    void remove(uint64_t id);
    
    // 更新（删除+插入）
    void update(uint64_t id, const Box& new_bounds);
    
    // 查询
    std::vector<uint64_t> query(const Box& region) const;
    std::vector<uint64_t> queryPoint(const Point& point) const;
    
    // 统计
    size_t size() const { return size_; }
    bool empty() const { return size_ == 0; }
    
    // 清空
    void clear();
    
    // 获取所有ID
    std::vector<uint64_t> allIds() const;
};

/// @brief 简单网格索引（用于对比或小规模数据）
class GridIndex {
private:
    struct Cell {
        std::vector<uint64_t> ids;
    };
    
    Box bounds_;
    double cell_size_;
    int rows_, cols_;
    std::vector<Cell> cells_;
    
    int getCellIndex(int row, int col) const {
        return row * cols_ + col;
    }
    
    void getCellRange(const Box& region, 
                     int& min_row, int& max_row,
                     int& min_col, int& max_col) const;
    
public:
    GridIndex(const Box& bounds, double cell_size);
    
    void insert(uint64_t id, const Box& bounds);
    void remove(uint64_t id);
    
    std::vector<uint64_t> query(const Box& region) const;
    
    void clear();
    size_t size() const;
};

} // namespace fluxcae

#endif // FLUXCAE_SPATIAL_INDEX_H

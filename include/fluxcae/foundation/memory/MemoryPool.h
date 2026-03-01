#ifndef FLUXCAE_MEMORY_POOL_H
#define FLUXCAE_MEMORY_POOL_H

#include <memory>
#include <vector>
#include <cstddef>
#include <cstdlib>
#include <new>

namespace fluxcae {

/// @brief 内存池分配器
/// @details 用于高性能分配大量小对象，减少new/delete开销
template<typename T>
class MemoryPool {
public:
    using value_type = T;
    using pointer = T*;
    using const_pointer = const T*;
    using reference = T&;
    using const_reference = const T&;
    using size_type = std::size_t;
    using difference_type = std::ptrdiff_t;

private:
    static constexpr size_t CHUNK_SIZE = 4096;  // 每块的大小
    static constexpr size_t OBJECTS_PER_CHUNK = CHUNK_SIZE / sizeof(T);
    
    std::vector<T*> chunks_;           // 已分配的内存块
    std::vector<T*> free_list_;       // 空闲对象列表
    
public:
    MemoryPool() noexcept = default;
    ~MemoryPool() { clear(); }
    
    // 禁止拷贝
    MemoryPool(const MemoryPool&) = delete;
    MemoryPool& operator=(const MemoryPool&) = delete;
    
    // 分配对象
    T* allocate() {
        if (free_list_.empty()) {
            allocate_chunk();
        }
        
        T* obj = free_list_.back();
        free_list_.pop_back();
        return obj;
    }
    
    // 分配多个对象
    T* allocate(size_type n) {
        if (n == 1) {
            return allocate();
        }
        
        // 简化实现：多次分配
        // TODO: 支持批量分配
        T* first = nullptr;
        T* last = nullptr;
        
        for (size_type i = 0; i < n; ++i) {
            T* obj = allocate();
            if (!first) first = obj;
            if (last) {
                // 构建链表（如果需要）
            }
            last = obj;
        }
        
        return first;
    }
    
    // 释放对象
    void deallocate(T* p) {
        if (!p) return;
        
        // 将对象加入空闲列表
        // 不调用析构函数，由使用者负责
        free_list_.push_back(p);
    }
    
    // 构造对象并返回指针
    template<typename... Args>
    T* construct(Args&&... args) {
        T* obj = allocate();
        new (obj) T(std::forward<Args>(args)...);
        return obj;
    }
    
    // 销毁对象
    void destroy(T* p) {
        if (p) {
            p->~T();
            deallocate(p);
        }
    }
    
    // 清空所有内存
    void clear() {
        for (T* chunk : chunks_) {
            std::free(chunk);
        }
        chunks_.clear();
        free_list_.clear();
    }
    
    // 当前空闲对象数
    size_t freeCount() const { return free_list_.size(); }
    
    // 已分配对象数
    size_t allocatedCount() const {
        return chunks_.size() * OBJECTS_PER_CHUNK - free_list_.size();
    }
    
    // 内存块大小
    size_type max_size() const noexcept {
        return OBJECTS_PER_CHUNK * 1000;  // 估计值
    }

private:
    void allocate_chunk() {
        T* chunk = static_cast<T*>(std::malloc(CHUNK_SIZE));
        if (!chunk) {
            throw std::bad_alloc();
        }
        
        chunks_.push_back(chunk);
        
        // 将新块的所有对象加入空闲列表
        for (size_t i = 0; i < OBJECTS_PER_CHUNK; ++i) {
            free_list_.push_back(&chunk[i]);
        }
    }
};

/// @brief 对象Arena分配器
/// @details 更简单的Arena实现，适合对象生命周期相似的场景
class ObjectArena {
private:
    struct Block {
        char* data;
        size_t size;
        size_t used;
        
        Block(size_t block_size) : size(block_size), used(0) {
            data = static_cast<char*>(std::malloc(block_size));
            if (!data) throw std::bad_alloc();
        }
        
        ~Block() { std::free(data); }
        
        bool hasSpace(size_t bytes) const { return used + bytes <= size; }
        
        void* allocate(size_t bytes) {
            void* ptr = &data[used];
            used += bytes;
            return ptr;
        }
        
        void reset() { used = 0; }
    };
    
    std::vector<std::unique_ptr<Block>> blocks_;
    size_t default_block_size_;
    size_t total_allocated_ = 0;

public:
    explicit ObjectArena(size_t default_size = 64 * 1024) 
        : default_block_size_(default_size) {
        blocks_.emplace_back(std::make_unique<Block>(default_size_));
    }
    
    ~ObjectArena() = default;
    
    // 禁止拷贝
    ObjectArena(const ObjectArena&) = delete;
    ObjectArena& operator=(const ObjectArena&) = delete;
    
    // 分配内存
    void* allocate(size_t bytes) {
        // 对齐到8字节
        bytes = (bytes + 7) & ~7;
        
        if (!blocks_.back()->hasSpace(bytes)) {
            // 分配新块
            size_t new_size = std::max(default_block_size_, bytes);
            blocks_.emplace_back(std::make_unique<Block>(new_size));
        }
        
        total_allocated_ += bytes;
        return blocks_.back()->allocate(bytes);
    }
    
    // 构造对象
    template<typename T, typename... Args>
    T* create(Args&&... args) {
        void* mem = allocate(sizeof(T));
        return new (mem) T(std::forward<Args>(args)...);
    }
    
    // 销毁对象
    template<typename T>
    void destroy(T* p) {
        p->~T();
        // 内存不会释放，回到arena
    }
    
    // 重置arena（销毁所有对象）
    void reset() {
        for (auto& block : blocks_) {
            block->reset();
        }
        total_allocated_ = 0;
    }
    
    // 获取总分配内存
    size_t totalAllocated() const { return total_allocated_; }
};

} // namespace fluxcae

#endif // FLUXCAE_MEMORY_POOL_H

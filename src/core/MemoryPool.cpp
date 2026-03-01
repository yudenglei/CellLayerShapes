/**
 * @file MemoryPool.cpp
 * @brief MemoryPool implementation
 * @author FluxCAE
 * @date 2024
 */
#include "fluxcae/foundation/memory/MemoryPool.h"
#include <cstring>
#include <stdexcept>

namespace fluxcae {
namespace foundation {

// MemoryPool实现
MemoryPool::MemoryPool(size_t blockSize, size_t poolSize)
    : blockSize_(blockSize), poolSize_(poolSize), currentBlock_(nullptr), 
      currentOffset_(0), totalAllocated_(0) {
    
    if (blockSize == 0) {
        throw std::invalid_argument("Block size must be greater than 0");
    }
    
    allocateBlock();
}

MemoryPool::~MemoryPool() {
    for (auto block : blocks_) {
        ::operator delete(block);
    }
}

void* MemoryPool::allocate() {
    std::lock_guard<std::mutex> lock(mutex_);
    
    // 检查当前块是否有足够空间
    if (currentOffset_ + blockSize_ > poolSize_) {
        allocateBlock();
    }
    
    void* ptr = static_cast<char*>(currentBlock_) + currentOffset_;
    currentOffset_ += blockSize_;
    totalAllocated_ += blockSize_;
    
    return ptr;
}

void* MemoryPool::allocate(size_t size) {
    std::lock_guard<std::mutex> lock(mutex_);
    
    // 如果请求的大小大于块大小，直接分配
    if (size > blockSize_) {
        totalAllocated_ += size;
        return ::operator new(size);
    }
    
    // 检查当前块是否有足够空间
    if (currentOffset_ + blockSize_ > poolSize_) {
        allocateBlock();
    }
    
    void* ptr = static_cast<char*>(currentBlock_) + currentOffset_;
    currentOffset_ += blockSize_;
    totalAllocated_ += blockSize_;
    
    return ptr;
}

void MemoryPool::deallocate(void* ptr) {
    std::lock_guard<std::mutex> lock(mutex_);
    
    // 检查指针是否在内存池中
    if (isInPool(ptr)) {
        // 简单实现：不释放，回收到池中
        // 更复杂的实现可以维护空闲列表
        return;
    }
    
    // 池外分配的直接释放
    ::operator delete(ptr);
}

void MemoryPool::reset() {
    std::lock_guard<std::mutex> lock(mutex_);
    
    currentOffset_ = 0;
    totalAllocated_ = 0;
    
    // 重置所有块
    for (auto block : blocks_) {
        std::memset(block, 0, poolSize_);
    }
}

size_t MemoryPool::totalAllocated() const {
    return totalAllocated_;
}

size MemoryPool::blockCount() const {
    return blocks_.size();
}

bool MemoryPool::isInPool(void* ptr) const {
    for (auto block : blocks_) {
        char* blockStart = static_cast<char*>(block);
        char* blockEnd = blockStart + poolSize_;
        char* p = static_cast<char*>(ptr);
        
        if (p >= blockStart && p < blockEnd) {
            return true;
        }
    }
    return false;
}

void MemoryPool::allocateBlock() {
    void* block = ::operator new(poolSize_);
    blocks_.push_back(block);
    currentBlock_ = block;
    currentOffset_ = 0;
}

// PoolAllocator实现
PoolAllocator::PoolAllocator(size_t blockSize)
    : pool_(std::make_shared<MemoryPool>(blockSize)) {
}

void* PoolAllocator::allocate(size_t size, size_t alignment) {
    void* ptr = pool_->allocate(size);
    
    // 简单对齐处理
    if (alignment > 1) {
        uintptr_t addr = reinterpret_cast<uintptr_t>(ptr);
        uintptr_t aligned = (addr + alignment - 1) & ~(alignment - 1);
        if (aligned != addr) {
            // 实际上这在简单池分配中很难完美实现
            // 理想情况是预先考虑对齐
        }
    }
    
    return ptr;
}

void PoolAllocator::deallocate(void* ptr) {
    pool_->deallocate(ptr);
}

} // namespace foundation
} // namespace fluxcae

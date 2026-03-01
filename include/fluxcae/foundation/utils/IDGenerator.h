/**
 * @file IDGenerator.h
 * @brief IDGenerator - 高效ID生成器
 * @author FluxCAE
 * @date 2024
 */
#pragma once

#include <atomic>
#include <cstdint>
#include <mutex>
#include <functional>

namespace fluxcae {
namespace foundation {

/**
 * @brief IDGenerator 高效的ID生成器
 * 
 * 支持：
 * - 线程安全的ID生成
 * - ID段分配
 * - ID复用
 * - 批量ID预分配
 */
class IDGenerator {
public:
    using IdType = uint64_t;

    /**
     * @brief 构造函数
     * @param startId 起始ID
     * @param blockSize 每次分配的块大小
     */
    explicit IDGenerator(IdType startId = 1, size_t blockSize = 1024);

    /**
     * @brief 析构函数
     */
    ~IDGenerator();

    // ===== ID生成 =====

    /**
     * @brief 生成下一个ID
     * @return 唯一ID
     */
    IdType next();

    /**
     * @brief 批量生成ID
     * @param count 数量
     * @return ID向量
     */
    std::vector<IdType> nextBatch(size_t count);

    // ===== ID段 =====

    /**
     * @brief 分配ID段
     * @param size 段大小
     * @return 段起始ID
     */
    IdType allocateBlock(size_t size);

    /**
     * @brief 释放ID段（可复用）
     * @param startId 起始ID
     * @param size 大小
     */
    void releaseBlock(IdType startId, size_t size);

    // ===== 状态查询 =====

    /// 获取已生成的ID数量
    IdType generatedCount() const { return generated_; }

    /// 获取当前可用ID数量
    size_t availableCount() const { return available_.size(); }

    /// 获取下一个ID（不生成）
    IdType peekNext() const { return nextId_.load(); }

    // ===== 重置 =====

    /**
     * @brief 重置生成器
     * @param startId 新的起始ID
     */
    void reset(IdType startId = 1);

    // ===== 静态方法 =====

    /**
     * @brief 生成UUID风格ID
     * @return UUID字符串
     */
    static std::string generateUUID();

    /**
     * @brief 生成简短ID字符串
     * @return 短ID字符串
     */
    static std::string generateShortId();

    /**
     * @brief 从字符串解析ID
     * @param str 字符串
     * @return ID值
     */
    static IdType parseId(const std::string& str);

private:
    std::atomic<IdType> nextId_;
    std::atomic<IdType> generated_;
    size_t blockSize_;
    
    std::mutex mutex_;
    std::vector<IdType> available_;
    
    void refill();
};

/**
 * @brief 线程局部ID生成器
 * 
 * 用于高性能场景，每个线程有独立的ID缓存
 */
class ThreadLocalIDGenerator {
public:
    /**
     * @brief 获取线程局部ID
     * @return 唯一ID
     */
    static IdType next();

    /**
     * @brief 批量获取ID
     * @param count 数量
     * @return ID向量
     */
    static std::vector<IdType> nextBatch(size_t count);

private:
    static thread_local IdType localNextId_;
    static thread_local std::vector<IdType> localCache_;
};

// 便捷类型定义
using IDGeneratorPtr = std::shared_ptr<IDGenerator>;

} // namespace foundation
} // namespace fluxcae

/**
 * @file ISerializer.h
 * @brief ISerializer - 序列化器接口
 * @author FluxCAE
 * @date 2024
 */
#pragma once

#include <string>
#include <memory>
#include <vector>
#include <cstdint>

namespace fluxcae {

// 前向声明
namespace domain {
namespace layout {
class Layout;
class Cell;
class Layer;
class Shape;
class Netlist;
}
}

namespace service {

/**
 * @brief SerializationFormat 序列化格式
 */
enum class SerializationFormat {
    Binary,     ///< 二进制格式
    JSON,       ///< JSON格式
    XML,        ///< XML格式
    GDSII,      ///< GDSII格式
    OASIS,      ///< OASIS格式
    DXF         ///< DXF格式
};

/**
 * @brief ISerializer 序列化器接口
 */
class ISerializer {
public:
    using Ptr = std::shared_ptr<ISerializer>;
    using LayoutPtr = std::shared_ptr<domain::layout::Layout>;

    /**
     * @brief 虚析构函数
     */
    virtual ~ISerializer() = default;

    // ===== 序列化 =====

    /**
     * @brief 序列化布局到文件
     * @param layout 布局指针
     * @param filePath 文件路径
     * @return 成功返回true
     */
    virtual bool serialize(LayoutPtr layout, const std::string& filePath) = 0;

    /**
     * @brief 序列化布局到字符串
     * @param layout 布局指针
     * @return 序列化后的字符串
     */
    virtual std::string serializeToString(LayoutPtr layout) = 0;

    /**
     * @brief 序列化布局到字节向量
     * @param layout 布局指针
     * @return 字节向量
     */
    virtual std::vector<uint8_t> serializeToBytes(LayoutPtr layout) = 0;

    // ===== 反序列化 =====

    /**
     * @brief 从文件反序列化布局
     * @param filePath 文件路径
     * @return 布局指针
     */
    virtual LayoutPtr deserialize(const std::string& filePath) = 0;

    /**
     * @brief 从字符串反序列化布局
     * @param data 字符串数据
     * @return 布局指针
     */
    virtual LayoutPtr deserializeFromString(const std::string& data) = 0;

    /**
     * @brief 从字节向量反序列化布局
     * @param data 字节数据
     * @return 布局指针
     */
    virtual LayoutPtr deserializeFromBytes(const std::vector<uint8_t>& data) = 0;

    // ===== 格式信息 =====

    /// 获取序列化格式
    virtual SerializationFormat format() const = 0;

    /// 获取格式名称
    virtual const char* formatName() const = 0;

    /// 获取文件扩展名
    virtual const char* fileExtension() const = 0;

    /// 是否支持二进制格式
    virtual bool isBinary() const = 0;

    // ===== 选项 =====

    /// 是否压缩
    bool compressed() const { return compressed_; }
    void setCompressed(bool c) { compressed_ = c; }

    /// 压缩级别
    int compressionLevel() const { return compressionLevel_; }
    void setCompressionLevel(int level) { compressionLevel_ = level; }

    /// 版本
    uint32_t version() const { return version_; }
    void setVersion(uint32_t v) { version_ = v; }

protected:
    ISerializer() : compressed_(false), compressionLevel_(6), version_(1) {}

    bool compressed_;
    int compressionLevel_;
    uint32_t version_;
};

// 工厂函数
/**
 * @brief 创建二进制序列化器
 * @return 序列化器指针
 */
ISerializer::Ptr createBinarySerializer();

/**
 * @brief 创建JSON序列化器
 * @return 序列化器指针
 */
ISerializer::Ptr createJsonSerializer();

/**
 * @brief 创建GDSII序列化器
 * @return 序列化器指针
 */
ISerializer::Ptr createGDSSerializer();

/**
 * @brief 创建OASIS序列化器
 * @return 序列化器指针
 */
ISerializer::Ptr createOASISSerializer();

} // namespace service
} // namespace fluxcae

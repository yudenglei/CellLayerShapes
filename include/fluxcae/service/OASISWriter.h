/**
 * @file OASISWriter.h
 * @brief OASISWriter - OASIS格式写入器
 * @author FluxCAE
 * @date 2024
 */
#pragma once

#include "fluxcae/service/ISerializer.h"
#include <string>
#include <memory>
#include <functional>

namespace fluxcae {
namespace service {

/**
 * @brief OASISWriter OASIS格式写入器
 */
class OASISWriter : public ISerializer {
public:
    using LayoutPtr = std::shared_ptr<domain::layout::Layout>;

    /**
     * @brief 构造函数
     */
    OASISWriter();

    /**
     * @brief 析构函数
     */
    ~OASISWriter() override;

    // ===== ISerializer接口 =====

    bool serialize(LayoutPtr layout, const std::string& filePath) override;
    std::string serializeToString(LayoutPtr layout) override;
    std::vector<uint8_t> serializeToBytes(LayoutPtr layout) override;

    LayoutPtr deserialize(const std::string& filePath) override;
    LayoutPtr deserializeFromString(const std::string& data) override;
    LayoutPtr deserializeFromBytes(const std::vector<uint8_t>& data) override;

    SerializationFormat format() const override { return SerializationFormat::OASIS; }
    const char* formatName() const override { return "OASIS"; }
    const char* fileExtension() const override { return ".oas"; }
    bool isBinary() const override { return true; }

    // ===== OASIS特定选项 =====

    /// 设置是否写入校验和
    void setWriteChecksums(bool write) { writeChecksums_ = write; }
    bool writeChecksums() const { return writeChecksums_; }

    /// 设置压缩级别 (0-9)
    void setCompressionLevel(int level) { compressionLevel_ = level; }
    int compressionLevel() const { return compressionLevel_; }

    /// 设置是否写入属性
    void setWriteProperties(bool write) { writeProperties_ = write; }
    bool writeProperties() const { return writeProperties_; }

    // ===== 进度回调 =====

    void setProgressCallback(std::function<void(int)> callback);

private:
    bool writeChecksums_;
    int compressionLevel_;
    bool writeProperties_;

    std::function<void(int)> progressCallback_;
};

// 便捷函数
bool writeOASIS(std::shared_ptr<domain::layout::Layout> layout, const std::string& filePath);

} // namespace service
} // namespace fluxcae

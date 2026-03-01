/**
 * @file OASISReader.h
 * @brief OASISReader - OASIS格式读取器
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
 * @brief OASISReader OASIS格式读取器
 * 
 * OASIS (Open Artwork System Interchange Standard) 是一种
 * 比GDSII更紧凑的版图格式
 */
class OASISReader : public ISerializer {
public:
    using LayoutPtr = std::shared_ptr<domain::layout::Layout>;

    /**
     * @brief 构造函数
     */
    OASISReader();

    /**
     * @brief 析构函数
     */
    ~OASISReader() override;

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

    /// 设置是否验证校验和
    void setValidateChecksums(bool validate) { validateChecksums_ = validate; }
    bool validateChecksums() const { return validateChecksums_; }

    /// 设置是否严格模式
    void setStrictMode(bool strict) { strictMode_ = strict; }
    bool strictMode() const { return strictMode_; }

    // ===== 进度回调 =====

    void setProgressCallback(std::function<void(int)> callback);
    void setWarningCallback(std::function<void(const std::string&)> callback);

    // ===== 统计信息 =====

    size_t cellCount() const { return cellCount_; }
    size_t shapeCount() const { return shapeCount_; }
    size_t warningCount() const { return warningCount_; }

private:
    bool validateChecksums_;
    bool strictMode_;

    std::function<void(int)> progressCallback_;
    std::function<void(const std::string&)> warningCallback_;

    size_t cellCount_;
    size_t shapeCount_;
    size_t warningCount_;
};

// 便捷函数
std::shared_ptr<domain::layout::Layout> readOASIS(const std::string& filePath);

} // namespace service
} // namespace fluxcae

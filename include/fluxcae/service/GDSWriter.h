/**
 * @file GDSWriter.h
 * @brief GDSWriter - GDSII格式写入器
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
 * @brief GDSWriter GDSII格式写入器
 */
class GDSWriter : public ISerializer {
public:
    using LayoutPtr = std::shared_ptr<domain::layout::Layout>;

    /**
     * @brief 构造函数
     */
    GDSWriter();

    /**
     * @brief 析构函数
     */
    ~GDSWriter() override;

    // ===== ISerializer接口 =====

    bool serialize(LayoutPtr layout, const std::string& filePath) override;
    std::string serializeToString(LayoutPtr layout) override;
    std::vector<uint8_t> serializeToBytes(LayoutPtr layout) override;

    LayoutPtr deserialize(const std::string& filePath) override;
    LayoutPtr deserializeFromString(const std::string& data) override;
    LayoutPtr deserializeFromBytes(const std::vector<uint8_t>& data) override;

    SerializationFormat format() const override { return SerializationFormat::GDSII; }
    const char* formatName() const override { return "GDSII"; }
    const char* fileExtension() const override { return ".gds"; }
    bool isBinary() const override { return true; }

    // ===== GDS特定选项 =====

    /// 设置坐标单位（数据库单位）
    void setDatabaseUnit(double unit) { databaseUnit_ = unit; }
    double databaseUnit() const { return databaseUnit_; }

    /// 设置用户单位
    void setUserUnit(double unit) { userUnit_ = unit; }
    double userUnit() const { return userUnit_; }

    /// 设置是否写入属性
    void setWriteAttributes(bool write) { writeAttributes_ = write; }
    bool writeAttributes() const { return writeAttributes_; }

    /// 设置是否写入时间戳
    void setWriteTimestamps(bool write) { writeTimestamps_ = write; }
    bool writeTimestamps() const { return writeTimestamps_; }

    // ===== 进度回调 =====

    /**
     * @brief 注册进度回调
     * @param callback 回调函数
     */
    void setProgressCallback(std::function<void(int)> callback);

private:
    double databaseUnit_;
    double userUnit_;
    bool writeAttributes_;
    bool writeTimestamps_;

    std::function<void(int)> progressCallback_;
};

// 便捷函数
/**
 * @brief 写入GDSII文件
 * @param layout 布局指针
 * @param filePath 文件路径
 * @return 成功返回true
 */
bool writeGDS(std::shared_ptr<domain::layout::Layout> layout, const std::string& filePath);

} // namespace service
} // namespace fluxcae

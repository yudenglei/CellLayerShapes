/**
 * @file GDSReader.h
 * @brief GDSReader - GDSII格式读取器
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
 * @brief GDSReader GDSII格式读取器
 * 
 * 支持读取GDSII格式的版图文件
 */
class GDSReader : public ISerializer {
public:
    using LayoutPtr = std::shared_ptr<domain::layout::Layout>;

    /**
     * @brief 构造函数
     */
    GDSReader();

    /**
     * @brief 析构函数
     */
    ~GDSReader() override;

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

    /// 设置是否读取单元属性
    void setReadCellAttributes(bool read) { readCellAttributes_ = read; }
    bool readCellAttributes() const { return readCellAttributes_; }

    /// 设置是否读取元素属性
    void setReadElementAttributes(bool read) { readElementAttributes_ = read; }
    bool readElementAttributes() const { return readElementAttributes_; }

    /// 设置坐标单位（数据库单位）
    void setDatabaseUnit(double unit) { databaseUnit_ = unit; }
    double databaseUnit() const { return databaseUnit_; }

    /// 设置用户单位
    void setUserUnit(double unit) { userUnit_ = unit; }
    double userUnit() const { return userUnit_; }

    // ===== 进度回调 =====

    /**
     * @brief 注册进度回调
     * @param callback 回调函数，参数：进度百分比(0-100)
     */
    void setProgressCallback(std::function<void(int)> callback);

    /**
     * @brief 注册警告回调
     * @param callback 回调函数，参数：警告消息
     */
    void setWarningCallback(std::function<void(const std::string&)> callback);

    // ===== 统计信息 =====

    /// 获取读取的单元数量
    size_t cellCount() const { return cellCount_; }

    /// 获取读取的图形数量
    size_t shapeCount() const { return shapeCount_; }

    /// 获取读取的文本数量
    size_t textCount() const { return textCount_; }

    /// 获取警告数量
    size_t warningCount() const { return warningCount_; }

private:
    bool readCellAttributes_;
    bool readElementAttributes_;
    double databaseUnit_;
    double userUnit_;

    std::function<void(int)> progressCallback_;
    std::function<void(const std::string&)> warningCallback_;

    size_t cellCount_;
    size_t shapeCount_;
    size_t textCount_;
    size_t warningCount_;
};

// 便捷函数
/**
 * @brief 读取GDSII文件
 * @param filePath 文件路径
 * @return 布局指针
 */
std::shared_ptr<domain::layout::Layout> readGDS(const std::string& filePath);

} // namespace service
} // namespace fluxcae

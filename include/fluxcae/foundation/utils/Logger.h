#ifndef FLUXCAE_LOGGER_H
#define FLUXCAE_LOGGER_H

#include <string>
#include <memory>
#include <fstream>
#include <mutex>
#include <sstream>
#include <iostream>
#include <vector>
#include <functional>

namespace fluxcae {

/// @brief 日志级别
enum class LogLevel {
    Debug = 0,
    Info = 1,
    Warning = 2,
    Error = 3,
    Fatal = 4
};

/// @brief 日志消息
struct LogMessage {
    LogLevel level;
    std::string message;
    std::string file;
    std::string function;
    int line;
    std::string timestamp;
    
    LogMessage(LogLevel lvl, const std::string& msg,
                const std::string& f = "", const std::string& func = "", int l = 0)
        : level(lvl), message(msg), file(f), function(func), line(l) {}
};

/// @brief 日志输出器接口
class ILogAppender {
public:
    virtual ~ILogAppender() = default;
    virtual void append(const LogMessage& msg) = 0;
};

/// @brief 控制台输出器
class ConsoleAppender : public ILogAppender {
private:
    bool use_colors_ = true;
    
public:
    ConsoleAppender(bool colors = true) : use_colors_(colors) {}
    
    void append(const LogMessage& msg) override;
};

/// @brief 文件输出器
class FileAppender : public ILogAppender {
private:
    std::ofstream file_;
    std::mutex mutex_;
    
public:
    FileAppender(const std::string& filename);
    ~FileAppender();
    
    void append(const LogMessage& msg) override;
    
    bool isOpen() const { return file_.is_open(); }
};

/// @brief 日志系统
class Logger {
private:
    static std::unique_ptr<Logger> instance_;
    static std::mutex init_mutex_;
    
    LogLevel min_level_ = LogLevel::Debug;
    std::vector<std::unique_ptr<ILogAppender>> appenders_;
    std::mutex mutex_;
    bool initialized_ = false;
    
    Logger() = default;
    
    void initDefaultAppenders();
    std::string formatMessage(const LogMessage& msg) const;
    std::string levelToString(LogLevel level) const;
    
public:
    // 单例
    static Logger& get();
    static void init();
    static void shutdown();
    
    // 不可拷贝
    Logger(const Logger&) = delete;
    Logger& operator=(const Logger&) = delete;
    
    // 配置
    void setLevel(LogLevel level) { min_level_ = level; }
    LogLevel level() const { return min_level_; }
    
    void addAppender(std::unique_ptr<ILogAppender> appender);
    void clearAppenders();
    
    // 记录日志
    void log(LogLevel level, const std::string& message,
              const std::string& file = "", 
              const std::string& function = "",
              int line = 0);
    
    // 便捷方法
    void debug(const std::string& msg, const std::string& file = "", 
               const std::string& func = "", int line = 0);
    void info(const std::string& msg, const std::string& file = "", 
              const std::string& func = "", int line = 0);
    void warning(const std::string& msg, const std::string& file = "", 
                 const std::string& func = "", int line = 0);
    void error(const std::string& msg, const std::string& file = "", 
               const std::string& func = "", int line = 0);
    void fatal(const std::string& msg, const std::string& file = "", 
                const std::string& func = "", int line = 0);
};

// 便捷宏
#define FLUXCAE_LOG_DEBUG(msg) fluxcae::Logger::get().debug(msg, __FILE__, __FUNCTION__, __LINE__)
#define FLUXCAE_LOG_INFO(msg) fluxcae::Logger::get().info(msg, __FILE__, __FUNCTION__, __LINE__)
#define FLUXCAE_LOG_WARNING(msg) fluxcae::Logger::get().warning(msg, __FILE__, __FUNCTION__, __LINE__)
#define FLUXCAE_LOG_ERROR(msg) fluxcae::Logger::get().error(msg, __FILE__, __FUNCTION__, __LINE__)
#define FLUXCAE_LOG_FATAL(msg) fluxcae::Logger::get().fatal(msg, __FILE__, __FUNCTION__, __LINE__)

} // namespace fluxcae

#endif // FLUXCAE_LOGGER_H

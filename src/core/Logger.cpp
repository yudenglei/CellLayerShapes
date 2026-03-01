/**
 * @file Logger.cpp
 * @brief Logger implementation
 * @author FluxCAE
 * @date 2024
 */
#include "fluxcae/foundation/utils/Logger.h"
#include <iostream>
#include <chrono>
#include <iomanip>
#include <sstream>
#include <thread>

namespace fluxcae {
namespace foundation {

// Logger实现
Logger& Logger::instance() {
    static Logger logger;
    return logger;
}

Logger::Logger() : minLevel_(LogLevel::Debug), output_(Output::Both) {
    // 默认添加控制台输出
    appenders_.push_back(std::make_shared<ConsoleAppender>());
}

Logger::~Logger() {
    for (auto& appender : appenders_) {
        appender->flush();
    }
}

void Logger::log(LogLevel level, const std::string& message) {
    if (level < minLevel_) {
        return;
    }

    LogEntry entry;
    entry.timestamp = std::chrono::system_clock::now();
    entry.level = level;
    entry.message = message;
    entry.threadId = std::this_thread::get_id();

    for (auto& appender : appenders_) {
        appender->append(entry);
    }
}

void Logger::debug(const std::string& message) {
    log(LogLevel::Debug, message);
}

void Logger::info(const std::string& message) {
    log(LogLevel::Info, message);
}

void Logger::warning(const std::string& message) {
    log(LogLevel::Warning, message);
}

void Logger::error(const std::string& message) {
    log(LogLevel::Error, message);
}

void Logger::critical(const std::string& message) {
    log(LogLevel::Critical, message);
}

void Logger::addAppender(AppenderPtr appender) {
    appenders_.push_back(appender);
}

void Logger::setMinLevel(LogLevel level) {
    minLevel_ = level;
}

void Logger::setOutput(Output output) {
    output_ = output;
}

const char* Logger::levelToString(LogLevel level) {
    switch (level) {
        case LogLevel::Debug:    return "DEBUG";
        case LogLevel::Info:     return "INFO ";
        case LogLevel::Warning:  return "WARN ";
        case LogLevel::Error:    return "ERROR";
        case LogLevel::Critical: return "CRIT ";
        default:                 return "UNKNOWN";
    }
}

// ConsoleAppender实现
void ConsoleAppender::append(const LogEntry& entry) {
    std::ostringstream oss;
    oss << "[" << formatTime(entry.timestamp) << "] "
        << "[" << Logger::levelToString(entry.level) << "] "
        << entry.message;
    
    if (entry.level >= LogLevel::Error) {
        std::cerr << oss.str() << std::endl;
    } else {
        std::cout << oss.str() << std::endl;
    }
}

void ConsoleAppender::flush() {
    std::cout.flush();
    std::cerr.flush();
}

std::string ConsoleAppender::formatTime(const std::chrono::system_clock::time_point& tp) {
    auto time = std::chrono::system_clock::to_time_t(tp);
    auto ms = std::chrono::duration_cast<std::chrono::milliseconds>(
        tp.time_since_epoch()) % 1000;
    
    std::ostringstream oss;
    oss << std::put_time(std::localtime(&time), "%Y-%m-%d %H:%M:%S")
        << '.' << std::setfill('0') << std::setw(3) << ms.count();
    return oss.str();
}

// FileAppender实现
FileAppender::FileAppender(const std::string& filename) 
    : filename_(filename), file_(filename, std::ios::app) {
}

FileAppender::~FileAppender() {
    flush();
}

void FileAppender::append(const LogEntry& entry) {
    std::ostringstream oss;
    oss << "[" << formatTime(entry.timestamp) << "] "
        << "[" << Logger::levelToString(entry.level) << "] "
        << entry.message
        << std::endl;
    
    std::lock_guard<std::mutex> lock(mutex_);
    if (file_.is_open()) {
        file_ << oss.str();
    }
}

void FileAppender::flush() {
    std::lock_guard<std::mutex> lock(mutex_);
    if (file_.is_open()) {
        file_.flush();
    }
}

std::string FileAppender::formatTime(const std::chrono::system_clock::time_point& tp) {
    auto time = std::chrono::system_clock::to_time_t(tp);
    auto ms = std::chrono::duration_cast<std::chrono::milliseconds>(
        tp.time_since_epoch()) % 1000;
    
    std::ostringstream oss;
    oss << std::put_time(std::localtime(&time), "%Y-%m-%d %H:%M:%S")
        << '.' << std::setfill('0') << std::setw(3) << ms.count();
    return oss.str();
}

} // namespace foundation
} // namespace fluxcae

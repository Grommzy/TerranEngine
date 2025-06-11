#ifndef LOG_H
#define LOG_H

#include <iostream>
#include <chrono>
#include <format>
#include <string_view>

// Compile-Time Log Level.
#ifndef TE_LOG_LEVEL
    #define TE_LOG_LEVEL 3      // 0=Error, 1=Warn, 2=Info (Default), 3=Debug
#endif

namespace TerranEngine
{
    enum class LogLevel : int
    {
        Error = 0,
        Warn = 1,
        Info = 2,
        Debug = 3
    };

    namespace Detail
    {
        // Tag strings and ANSI colour codes (8-bit, reset = "\033[0m").
        inline constexpr std::string_view LevelTag(LogLevel lvl)
        {
            switch (lvl)
            {
                case LogLevel::Error:   return "ERROR";
                case LogLevel::Warn:    return "WARN" ;
                case LogLevel::Info:    return "INFO" ;
                default:                return "DEBUG";
            }
        }

        inline constexpr std::string_view Colour(LogLevel lvl)
        {
            switch (lvl)
            {
                case LogLevel::Error:   return "\033[31m";  // Red
                case LogLevel::Warn:    return "\033[33m";  // Yellow
                case LogLevel::Info:    return "\033[36m";  // Cyan
                default:                return "\033[90m";  // Bright grey
            }
        }

        // Log sink.
        template <typename... Args>
        inline void Log(LogLevel lvl, std::string_view format, Args &&... args)
        {
            if (static_cast<int>(lvl) > TE_LOG_LEVEL)
            {
                return;
            }

            // Timestep in microseconds since epoch.
            const auto now = std::chrono::system_clock::now();
            const auto us  = std::chrono::duration_cast<std::chrono::microseconds>(now.time_since_epoch()).count();

            // Format message.
            const std::string body = std::vformat(format, std::make_format_args(args...));

            // Pick output stream
            std::ostream& out = (lvl == LogLevel::Error || lvl == LogLevel::Warn) ? std::cerr : std::cout;

            // Compose final line `[time] TAG message`
            out << Colour(lvl)
                << '[' << us << "] "
                << LevelTag(lvl) << " "
                << body
                << "\033[0m\n";
            out.flush();
        }
    } // Detail namespace.
} // TerranEngine namespace.

#define TE_LOG_ERROR(format, ...) ::TerranEngine::Detail::Log(::TerranEngine::LogLevel::Error, (format), ##__VA_ARGS__)
#define TE_LOG_WARN(format,  ...) ::TerranEngine::Detail::Log(::TerranEngine::LogLevel::Warn,  (format), ##__VA_ARGS__)
#define TE_LOG_INFO(format,  ...) ::TerranEngine::Detail::Log(::TerranEngine::LogLevel::Info,  (format), ##__VA_ARGS__)
#define TE_LOG_DEBUG(format, ...) ::TerranEngine::Detail::Log(::TerranEngine::LogLevel::Debug, (format), ##__VA_ARGS__)

#endif // LOG_H
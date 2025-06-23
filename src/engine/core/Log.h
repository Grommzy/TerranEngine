#ifndef TERRANENGINE_LOG_H
#define TERRANENGINE_LOG_H

#ifndef TE_LOG_LEVEL
    #define TE_LOG_LEVEL 3 // 0=Error, 1=Warn, 2=Info (Default), 3=Debug
#endif

#include <chrono>
#include <format>
#include <iostream>
#include <string_view>

namespace TerranEngine
{
    /** Severity levels for logging. */
    enum class LogLevel : int
    {
        ERROR = 0,
        WARN = 1,
        INFO = 2,
        DEBUG = 3
    };

    namespace Detail
    {
        /** Returns the ANSI colour code for a given log level. */
        inline std::string_view LevelColour(const LogLevel level) noexcept
        {
            switch (level)
            {
                case LogLevel::ERROR: return "\033[1;31m"; // Red
                case LogLevel::WARN:  return "\033[1;33m"; // Yellow
                case LogLevel::INFO:  return "\033[1;32m"; // Green
                case LogLevel::DEBUG: return "\033[1;34m"; // Blue
            }
            return "\033[0m";
        }

        /** Returns the textual tag for a given log level. */
        inline std::string_view LevelTag(const LogLevel level) noexcept
        {
            switch (level)
            {
                case LogLevel::ERROR: return "[ERROR]";
                case LogLevel::WARN:  return "[WARN] ";
                case LogLevel::INFO:  return "[INFO] ";
                case LogLevel::DEBUG: return "[DEBUG]";
            }
            return "[LOG]  ";
        }

        /**
         * Core logger implementation.
         *
         * Builds a formatted message prefixed by:
         *  - ANSI colour escape
         *  - [<microseconds since epoch>]
         *  - level tag
         * Then resets colour and flushes.
         */
        template<LogLevel level, typename... Args>
        inline void Log(const std::string_view formatString, Args&&... args) noexcept
        {
            const auto now = std::chrono::system_clock::now();
            const auto ms  = std::chrono::duration_cast<std::chrono::milliseconds>(now.time_since_epoch()) % 1000;

            // Convert to local calender time.
            std::time_t time = std::chrono::system_clock::to_time_t(now);
            std::tm tmLoc {};

            #if defined(_WIN32)
                localtime_s(&tmLoc, &time); // Windows
            #else
                localtime_r(&time, &tmLoc); // POSIX
            #endif

            // Build the time string. -------"HH:MM:SS"-----'.'-----------------------------------------"MsMsMs"------' '------------------------"PM/PM"
            std::ostringstream timeStr;
            timeStr << std::put_time(&tmLoc, "%I:%M:%S") << '.' << std::setw(3) << std::setfill('0') << ms.count() << ' ' << std::put_time(&tmLoc, "%p");

            const std::string timeString = timeStr.str();

            const std::string body = std::vformat(formatString, std::make_format_args(args...));

            std::ostream& outStream = (level == LogLevel::ERROR ? std::cerr : std::cout);

            outStream << LevelColour(level) << '[' << timeString << "] " << LevelTag(level) << ' ' << body << "\033[0m\n";
            outStream.flush();
        }
    }

    #if TE_LOG_LEVEL >= 0
        #define TE_LOG_ERROR(format, ...) ::TerranEngine::Detail::Log<TerranEngine::LogLevel::ERROR>(format, ##__VA_ARGS__)
    #else
        #define TE_LOG_ERROR(format, ...) ((void)0)
    #endif

    #if TE_LOG_LEVEL >= 1
        #define TE_LOG_WARN(format, ...) ::TerranEngine::Detail::Log<TerranEngine::LogLevel::WARN>(format, ##__VA_ARGS__)
    #else
        #define TE_LOG_WARN(format, ...) ((void)0)
    #endif

    #if TE_LOG_LEVEL >= 2
        #define TE_LOG_INFO(format, ...) ::TerranEngine::Detail::Log<TerranEngine::LogLevel::INFO>(format, ##__VA_ARGS__)
    #else
        #define TE_LOG_INFO(format, ...) ((void)0)
    #endif

    #if TE_LOG_LEVEL >= 3
        #define TE_LOG_DEBUG(format, ...) ::TerranEngine::Detail::Log<TerranEngine::LogLevel::DEBUG>(format, ##__VA_ARGS__)
    #else
        #define TE_LOG_DEBUG(format, ...) ((void)0)
    #endif
}

#endif // TERRANENGINE_LOG_H
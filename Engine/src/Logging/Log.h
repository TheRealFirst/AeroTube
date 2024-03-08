#pragma once
#include <stdint.h>

#ifdef AT_DEBUG
#define LOG_WARN_ENABLED 1
#define LOG_INFO_ENABLED 1
#define LOG_DEBUG_ENABLED 1
#define LOG_TRACE_ENABLED 1
#endif

typedef enum log_level{
    LOG_LEVEL_FATAL = 0,
    LOG_LEVEL_ERROR = 1,
    LOG_LEVEL_WARN = 2,
    LOG_LEVEL_INFO = 3,
    LOG_LEVEL_DEBUG = 4,
    LOG_LEVEL_TRACE = 5
} log_level;

// Logs a fatal-level message
#define LOG_FATAL(message, ...) Log::get()->log_output(LOG_LEVEL_FATAL, message, ##__VA_ARGS__);

#ifndef LOG_ERROR
// Logs an error-level message.
#define LOG_ERROR(message, ...) Log::get()->log_output(LOG_LEVEL_ERROR, message, ##__VA_ARGS__);
#endif

#if LOG_WARN_ENABLED == 1
// Logs an warning-level message.
#define LOG_WARN(message, ...) Log::get()->log_output(LOG_LEVEL_WARN, message, ##__VA_ARGS__);
#else
// Does nothing when LOG_WARN_ENABLED != 1
#define LOG_WARN(message, ...)
#endif

#if LOG_INFO_ENABLED == 1
// Logs an info-level message.
#define LOG_INFO(message, ...) Log::get()->log_output(LOG_LEVEL_INFO, message, ##__VA_ARGS__);
#else
// Does nothing when LOG_INFO_ENABLED != 1
#define LOG_INFO(message, ...)
#endif

#if LOG_DEBUG_ENABLED == 1
// Logs an debug-level message.
#define LOG_DEBUG(message, ...) Log::get()->log_output(LOG_LEVEL_DEBUG, message, ##__VA_ARGS__);
#else
// Does nothing when LOG_DEBUG_ENABLED != 1
#define LOG_DEBUG(message, ...)
#endif

#if LOG_TRACE_ENABLED == 1
// Logs an trace-level message.
#define LOG_TRACE(message, ...) Log::get()->log_output(LOG_LEVEL_TRACE, message, ##__VA_ARGS__);
#else
// Does nothing when LOG_TRACE_ENABLED != 1
#define LOG_TRACE(message, ...)
#endif

class Log
{
public:
    bool initialize_logging();
    void shutdown_logging();

    void log_output(log_level level, const char* message, ...);
public:
    static Log* get();
};

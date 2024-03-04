#include "Log.h"

// TODO: temporary
#include <iostream>
#include <stdio.h>
#include <string.h>
#include <stdarg.h>
#include "ccprintf.h"

bool Log::initialize_logging(){
    // TODO: create log file
    return true;
}

void Log::shutdown_logging(){
    // TODO: cleanup logging/write queued entries
}

void Log::log_output(log_level level, const char* message, ...){
    const char* level_strings[6] = {"[FATAL]: ", "[ERROR]: ", "[WARN]: ", "[INFO]: ", "[DEBUG]: ", "[TRACE]: "};

    // Technically imposes a 32k character limit on a single log entry
    const int32_t msg_length = 32000;
    char out_message[msg_length];
    memset(out_message, 0, sizeof(out_message));

    // Format original message.
    // NOTE: Oddly enough, MS's headers override the GCC/Clang va_list type with a "typedef char* va_list" in some
    // cases, and as a result throws a strange error here. The workaround for now is to just use __builtin_va_list,
    // which is the type GCC/Clang's va_start expects.
    va_list arg_ptr;
    va_start(arg_ptr, message);
    vsnprintf(out_message, msg_length, message, arg_ptr);
    va_end(arg_ptr);

    char out_message2[msg_length];
    sprintf_s(out_message2, msg_length, "%s%s\n", level_strings[level], out_message);

    switch (level)
    {
    case (LOG_LEVEL_FATAL):
        {
            std::cout << cc::red << out_message2 << std::endl;
            break;
        }
    case (LOG_LEVEL_ERROR):
        {
            std::cout << cc::red << out_message2 << std::endl;
            break;
        }
    case (LOG_LEVEL_WARN):
        {
            std::cout << cc::yellow << out_message2 << std::endl;
            break;
        }
    case (LOG_LEVEL_INFO):
        {
            std::cout << cc::normal << out_message2 << std::endl;
            break;
        }
    case (LOG_LEVEL_DEBUG):
        {
            std::cout << cc::cyan << out_message2 << std::endl;
            break;
        }
    case (LOG_LEVEL_TRACE):
        {
            std::cout << cc::green << out_message2 << std::endl;
            break;
        }
    }
}



Log* Log::get()
{
    static Log logger;
    return &logger;
}


void report_assertion_failure(const char* expression, const char* message, const char* file, int32_t line){
    Log::get()->log_output(LOG_LEVEL_FATAL, "Assertion Failure: %s, message: '%s', in file: %s, line: %d\n", expression, message, file, line);
}


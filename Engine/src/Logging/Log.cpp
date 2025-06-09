#include "atpch.h"
#include "Log.h"

#include <stdarg.h>
#include "ccprintf.h"
#include "asserts.h"


const char* filepath = "Engine.log";

bool initialize_logging() {
    std::ofstream file(filepath);
    file.close();

    // current date/time based on current system
    time_t now = time(0);

    // convert now to string form
    const char* dt = ctime(&now);

    print_to_file(filepath, "Programm started at: ");
    print_to_file(filepath, dt);

    
    return true;
}

void shutdown_logging() {
}

void log_output(log_level level, const char* message, ...) {
    const char* level_strings[6] = { "[FATAL]: ", "[ERROR]: ", "[WARN]: ", "[INFO]: ", "[DEBUG]: ", "[TRACE]: " };

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

    print_to_file(filepath, out_message2);

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

void report_assertion_failure(const char* expression, const char* message, const char* file, int32_t line) {
    log_output(LOG_LEVEL_FATAL, "Assertion Failure: %s, message: '%s', in file: %s, line: %d\n", expression, message, file, line);
}

void print_to_file(const char* filepath, const char* message) {
    std::ofstream file(filepath, std::ios::app);
    file << message << std::endl;
    file.close();
}

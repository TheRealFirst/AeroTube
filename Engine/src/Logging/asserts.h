#pragma once

// Disable assertions by commenting out the below line
#define ASSERTIONS_ENABLED

#ifdef ASSERTIONS_ENABLED
#if _MSC_VER
#include <intrin.h>
#define debugBreak() __debugbreak()
#else
#define debugBreak() __builtin_trap()
#endif

void report_assertion_failure(const char* expression, const char* message, const char* file, int32_t line);

#define AT_ASSERT(expr)                                                   \
    {                                                                   \
        if(expr){                                                       \
        }else{                                                          \
            report_assertion_failure(#expr, "", __FILE__, __LINE__);    \
            debugBreak();                                               \
        }                                                               \
    } 

#define ASSERT_MSG(expr, message)                                              \
    {                                                                           \
        if(expr){                                                               \
        }else{                                                                  \
            report_assertion_failure(#expr, message, __FILE__, __LINE__);       \
            debugBreak();                                                       \
        }                                                                       \
    } 

#ifdef AT_DEBUG
#define ASSERT_DEBUG(expr)                                             \
    {                                                                   \
        if(expr){                                                       \
        }else{                                                          \
            report_assertion_failure(#expr, "", __FILE__, __LINE__);    \
            debugBreak();                                               \
        }                                                               \
    } 
#else
#define ASSERT_DEBUG(expr) // Does nothing at all
#endif

#else
#define ASSERT(expr)               // Does nothing at all
#define ASSERT_MSG(expr, message)  // Does nothing at all
#define ASSERT_DEBUG(expr)         // Does nothing at all
#endif

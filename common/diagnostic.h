#pragma once

//
// Diagnostics control macros and options
//


//
// Enable asserts which can terminate the program when exceptional conditions are encountered
//
#if defined(AT_DIAG_ENABLE_ASSERT)
    #include <assert.h>
    #define AT_ASSERT(expr) assert(expr)
    #define AT_ASSERT_MSG(expr, format, ... ) if (!(expr)) { fprintf(stderr, format , ##__VA_ARGS__); assert(0); }
#else
    #define AT_ASSERT(expr)
    #define AT_ASSERT_MSG(expr, format, ...)
#endif // defined(AT_DIAG_ENABLE_ASSERT)

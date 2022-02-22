#pragma once

//
// Tracing will call specific methods to allow diagnostic visualization of various stages
// of the detection algorithm.
//
#if defined(AT_DIAG_ENABLE_TRACING)
    #define AT_TRACE(detector, callback, ...) if ((detector)->callback) { (detector)->callback(__VA_ARGS__); }
#else
    #define AT_TRACE(detector, callback, ...)
#endif



//
// Profiling the library with specific instrumented timestamps
//
#if defined(AT_DIAG_ENABLE_TIMESTAMPS)
    #define AT_TIMESTAMP(detector, name) if ((detector)->diag.timestamp) { (detector)->diag.timestamp((detector)->tp, name); }
#else
    #define AT_TIMESTAMP(detector, name)
#endif

//
// Enable asserts which can terminate the program when exceptional conditions are encountered
//
#if defined(AT_DIAG_ENABLE_ASSERT)
    #include <assert.h>
    #define AT_ASSERT(expr) assert(expr)
    #define AT_ASSERT_MSG(expr, format, ... ) if (!(expr)) { AT_ERROR_TEXT(format , ##__VA_ARGS__); assert(0); }
#else
    #define AT_ASSERT(expr)
    #define AT_ASSERT_MSG(expr, format, ...)
#endif // defined(AT_DIAG_ENABLE_ASSERT)

//
// Panic is a specific form of exit to maintain compatibility with previous exit conditions.
//   i.e.: functions like an assert but exits with a -1 exit code
//
#define AT_PANIC(format, ...) do { AT_ERROR_TEXT(format , ##__VA_ARGS__); exit(-1); } while(0);

//
// Textural line ending
//
#define AT_LINE_END "\n"

//
// Textual tracing/debugging
//
#if defined(AT_DIAG_ENABLE_TEXT)

    // Only write debug output if explicitly enabled
    #if defined(AT_DIAG_ENABLE_DEBUG)
    #define AT_DEBUG_TEXT(format, ...) do { fprintf(stderr, "[D] [%s:%d] " format AT_LINE_END , __FILE__ , __LINE__ , ##__VA_ARGS__ ); fflush(stderr); } while(0);
    #else
    #define AT_DEBUG_TEXT(format, ...)
    #endif

    // Stderr text diagnostics
    #if defined (AT_DIAG_TEXT_STDERR)
    #define AT_INFO_TEXT(format, ...) do { fprintf(stderr, "[I] [%s:%d] " format AT_LINE_END , __FILE__ , __LINE__ , ##__VA_ARGS__ ); fflush(stderr); } while(0);
    #define AT_ERROR_TEXT(format, ...) do { fprintf(stderr, "[E] [%s:%d] " format AT_LINE_END , __FILE__ , __LINE__ , ##__VA_ARGS__ ); fflush(stderr); } while(0);
    #define AT_WARN_TEXT(format, ...) do { fprintf(stderr, "[W] [%s:%d] " format AT_LINE_END , __FILE__ , __LINE__ , ##__VA_ARGS__ ); fflush(stderr); } while(0);
    #endif

#else
    // Disable text diagnostics
    #define AT_DEBUG_TEXT(format, ...)
    #define AT_INFO_TEXT(format, ...)
    #define AT_ERROR_TEXT(format, ...)
    #define AT_WARN_TEXT(format, ...)
#endif

#if defined(AT_DIAG_MATH)
    #define AT_DMATH_TEXT(format, ...) fprintf(stderr, "    [math] [%s:%d] " format , __FILE__ , __LINE__ , ##__VA_ARGS__ ); fflush(stderr);
    #define AT_DMATH0(format, ...)                  do { AT_DMATH_TEXT(format , ##__VA_ARGS__); fprintf(stderr, AT_LINE_END); } while(0);
    #define AT_DMATH_OF(a, format, ...)             do { AT_DMATH_TEXT(format , ##__VA_ARGS__); matd_print_flat(a,      " <- ", ""); fprintf(stderr, AT_LINE_END); } while(0);
    #define AT_DMATH(output, format, ...)           do { AT_DMATH_TEXT(format , ##__VA_ARGS__); matd_print_flat(output, " <- ", ""); fprintf(stderr, AT_LINE_END); } while(0);
    #define AT_DMATH1(output, a, format, ...)       do { AT_DMATH_TEXT(format , ##__VA_ARGS__); matd_print_flat(output, " <- ", ""); matd_print_flat(a, "a: ", ""); fprintf(stderr, AT_LINE_END); } while(0);
    #define AT_DMATH2(output, a, b, format, ...)    do { AT_DMATH_TEXT(format , ##__VA_ARGS__); matd_print_flat(output, " <- ", ""); matd_print_flat(a, "a: ", ""); matd_print_flat(b, ", b: ", ""); fprintf(stderr, AT_LINE_END); } while(0);
#else
    #define AT_DMATH0(format, ...)
    #define AT_DMATH_OF(a, format, ...)
    #define AT_DMATH(output, format, ...)
    #define AT_DMATH1(output, a, format, ...)
    #define AT_DMATH2(output, a, b, format, ...)
#endif

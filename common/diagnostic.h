//
// Created by Jacob on 1/28/22.
//
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
    #define AT_ASSERT(EXPR) assert(EXPR)
    #define AT_ASSERT_MSG(EXPR, FMT, ... ) if (!(EXPR)) { AT_ERROR_TEXT(FMT , ##__VA_ARGS__); assert(0); }
#else
    #define AT_ASSERT(EXPR)
    #define AT_ASSERT_MSG(EXPR, FMT, ...)
#endif // defined(AT_DIAG_ENABLE_ASSERT)

//
// Panic is a specific form of exit to maintain compatibility with previous exit conditions.
//   i.e.: functions like an assert but exits with a -1 exit code
//
#define AT_PANIC(FMT, ...) do { AT_ERROR_TEXT(FMT , ##__VA_ARGS__); exit(-1); } while(0);

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
    #define AT_DEBUG_TEXT(FMT, ...) do { fprintf(stderr, "[D] [%s:%d] " FMT AT_LINE_END , __FILE__ , __LINE__ , ##__VA_ARGS__ ); fflush(stderr); } while(0);
    #else
    #define AT_DEBUG_TEXT(FMT, ...)
    #endif

    // Stderr text diagnostics
    #if defined (AT_DIAG_TEXT_STDERR)
    #define AT_INFO_TEXT(FMT, ...) do { fprintf(stderr, "[I] [%s:%d] " FMT AT_LINE_END , __FILE__ , __LINE__ , ##__VA_ARGS__ ); fflush(stderr); } while(0);
    #define AT_ERROR_TEXT(FMT, ...) do { fprintf(stderr, "[E] [%s:%d] " FMT AT_LINE_END , __FILE__ , __LINE__ , ##__VA_ARGS__ ); fflush(stderr); } while(0);
    #define AT_WARN_TEXT(FMT, ...) do { fprintf(stderr, "[W] [%s:%d] " FMT AT_LINE_END , __FILE__ , __LINE__ , ##__VA_ARGS__ ); fflush(stderr); } while(0);
    #endif

#else
    // Disable text diagnostics
    #define AT_DEBUG_TEXT(FMT, ...)
    #define AT_INFO_TEXT(FMT, ...)
    #define AT_ERROR_TEXT(FMT, ...)
    #define AT_WARN_TEXT(FMT, ...)
#endif

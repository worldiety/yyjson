/*==============================================================================
 * Utilities for single thread test and benchmark.
 *
 * Copyright (C) 2018 YaoYuan <ibireme@gmail.com>.
 * Released under the MIT license (MIT).
 *============================================================================*/

#ifndef yy_test_utils_h
#define yy_test_utils_h

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <float.h>
#include <math.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdarg.h>

#ifdef _WIN32
#   include <windows.h>
#   include <io.h>
#   include <intrin.h>
#else
#   ifdef __APPLE__
#       include <TargetConditionals.h>
#       include <mach/mach_init.h>
#       include <mach/mach_time.h>
#       include <mach/thread_policy.h>
#       include <mach/thread_act.h>
#       include <sys/types.h>
#       include <sys/sysctl.h>
#       include <sys/utsname.h>
#   else
#       ifndef _GNU_SOURCE
#           define _GNU_SOURCE
#       endif
#       ifndef __USE_GNU
#           define __USE_GNU
#       endif
#   endif
#   include <sys/time.h>
#   include <pthread.h>
#   include <sched.h>
#   include <dirent.h>
#   include <sys/stat.h>
#endif

/* architecture */
#if defined(__x86_64) || defined(__x86_64__) || \
    defined(__amd64) || defined(__amd64__) || \
    defined(_M_AMD64) || defined(_M_X64)
#   define YY_ARCH_X64 1
#   define YY_ARCH_64  1
#elif defined(i386) || defined(__i386) || defined(__i386__) || \
    defined(_X86_) || defined(__X86__) || defined(_M_IX86) || \
    defined(__I86__) || defined(__IA32__) || defined(__THW_INTEL)
#   define YY_ARCH_X86 1
#   define YY_ARCH_32  1
#elif defined(__arm64) || defined(__arm64__) || \
    defined(__aarch64__) || defined(_M_ARM64)
#   define YY_ARCH_ARM64 1
#   define YY_ARCH_64    1
#elif defined(__arm) || defined(__arm__) || defined(_ARM_) || \
    defined(_ARM) || defined(_M_ARM) || defined(__TARGET_ARCH_ARM)
#   define YY_ARCH_ARM32 1
#   define YY_ARCH_32    1
#endif

#if !YY_ARCH_64 && YY_ARCH_32
#   if defined(_LP64) || defined(__LP64__) || defined(__64BIT__)
#       define YY_ARCH_64 1
#   endif
#endif

/* compiler builtin check (clang) */
#ifndef yy_has_builtin
#   ifdef __has_builtin
#       define yy_has_builtin(x) __has_builtin(x)
#   else
#       define yy_has_builtin(x) 0
#   endif
#endif

/* compiler attribute check (gcc/clang) */
#ifndef yy_has_attribute
#   ifdef __has_attribute
#       define yy_has_attribute(x) __has_attribute(x)
#   else
#       define yy_has_attribute(x) 0
#   endif
#endif

/* include check (gcc/clang) */
#ifndef yy_has_include
#   ifdef __has_include
#       define yy_has_include(x) __has_include(x)
#   else
#       define yy_has_include(x) 0
#   endif
#endif

/* inline */
#ifndef yy_inline
#   if _MSC_VER >= 1200
#       define yy_inline __forceinline
#   elif defined(_MSC_VER)
#       define yy_inline __inline
#   elif yy_has_attribute(always_inline) || __GNUC__ >= 4
#       define yy_inline __inline__ __attribute__((always_inline))
#   elif defined(__clang__) || defined(__GNUC__)
#       define yy_inline __inline__
#   elif defined(__cplusplus) || (__STDC__ >= 1 && __STDC_VERSION__ >= 199901L)
#       define yy_inline inline
#   else
#       define yy_inline
#   endif
#endif

/* noinline */
#ifndef yy_noinline
#   if _MSC_VER >= 1200
#       define yy_noinline __declspec(noinline)
#   elif yy_has_attribute(noinline) || __GNUC__ >= 4
#       define yy_noinline __attribute__((noinline))
#   else
#       define yy_noinline
#   endif
#endif

/* likely */
#ifndef yy_likely
#   if yy_has_builtin(__builtin_expect) || __GNUC__ >= 4
#       define yy_likely(expr) __builtin_expect(!!(expr), 1)
#   else
#       define yy_likely(expr) (expr)
#   endif
#endif

/* unlikely */
#ifndef yy_unlikely
#   if yy_has_builtin(__builtin_expect) || __GNUC__ >= 4
#       define yy_unlikely(expr) __builtin_expect(!!(expr), 0)
#   else
#       define yy_unlikely(expr) (expr)
#   endif
#endif

/* stdint */
#if YY_HAS_STDINT_H || yy_has_include(<stdint.h>) || \
    _MSC_VER >= 1600 || (__STDC__ >= 1 && __STDC_VERSION__ >= 199901L) || \
    defined(_STDINT_H) || defined(_STDINT_H_) || defined(__CLANG_STDINT_H) || \
    defined(_STDINT_H_INCLUDED)
#   include <stdint.h>
#elif defined(_MSC_VER)
#   if _MSC_VER < 1300
typedef signed char         int8_t;
typedef signed short        int16_t;
typedef signed int          int32_t;
typedef unsigned char       uint8_t;
typedef unsigned short      uint16_t;
typedef unsigned int        uint32_t;
typedef signed __int64      int64_t;
typedef unsigned __int64    uint64_t;
#   else
typedef signed __int8       int8_t;
typedef signed __int16      int16_t;
typedef signed __int32      int32_t;
typedef unsigned __int8     uint8_t;
typedef unsigned __int16    uint16_t;
typedef unsigned __int32    uint32_t;
typedef signed __int64      int64_t;
typedef unsigned __int64    uint64_t;
#   endif
#endif

/* stdbool */
#if YY_HAS_STDBOOL_H || yy_has_include(<stdbool.h>) || \
    _MSC_VER >= 1800 || (__STDC__ >= 1 && __STDC_VERSION__ >= 199901L)
#   include <stdbool.h>
#elif !defined(__bool_true_false_are_defined)
#   define __bool_true_false_are_defined 1
#   if defined(__cplusplus)
#       if defined(__GNUC__) && !defined(__STRICT_ANSI__)
#           define _Bool bool
#           if __cplusplus < 201103L
#               define bool bool
#               define false false
#               define true true
#           endif
#       endif
#   else
#       define bool unsigned char
#       define true 1
#       define false 0
#   endif
#endif

/* assert */
#define yy_assert(expr) \
    if (!(expr)) { \
        fprintf(stderr, "Assertion failed: %s (%s: %d)\n", #expr, __FILE__, __LINE__); \
        abort(); \
    };

#define yy_assertf(expr, ...) \
    if (!(expr)) { \
        fprintf(stderr, "Assertion failed: %s (%s: %d)\n", #expr, __FILE__, __LINE__); \
        fprintf(stderr, __VA_ARGS__); \
        fprintf(stderr, "\n"); \
        abort(); \
    };

/* test */
#if yy_has_include("yy_xctest.h")
#   include "yy_xctest.h"
#   define yy_test_case(name) \
        void name(void)
#else
#   define yy_test_case(name) \
    void name(void); \
    int main(int argc, const char * argv[]) { \
        name(); \
        return 0; \
    } \
    void name(void)
#endif



#ifdef __cplusplus
extern "C" {
#endif



/*==============================================================================
 * Type Defines
 *============================================================================*/

typedef float       f32;
typedef double      f64;
typedef int8_t      i8;
typedef uint8_t     u8;
typedef int16_t     i16;
typedef uint16_t    u16;
typedef int32_t     i32;
typedef uint32_t    u32;
typedef int64_t     i64;
typedef uint64_t    u64;
typedef size_t      usize;



/*==============================================================================
 * Timer
 *============================================================================*/

/** Structure holding a timestamp. */
typedef struct yy_time yy_time;

/** Get current wall time to a structure. */
static yy_inline void yy_time_get_current(yy_time *t);

/** Convert time structure to seconds. */
static yy_inline f64 yy_time_to_seconds(yy_time *t);

/** Get current wall time in seconds. */
static yy_inline f64 yy_time_get_seconds(void);

/** A high-resolution, low-overhead, fixed-frequency timer for benchmark. */
static yy_inline u64 yy_time_get_ticks(void);



/*==============================================================================
 * CPU
 *============================================================================*/

/** Try to increase the priority of the current thread.
    This method may used to reduce context switches of current thread. */
bool yy_cpu_setup_priority(void);

/** Let CPU 'spinning' for a while.
    This function may used to warm up CPU from power saving mode and
    stabilize the CPU frequency. */
void yy_cpu_spin(f64 second);

/** Measure current CPU frequency.
    This function may take about 1 second on 1GHz CPU.
    This function may returns inaccurate result in debug mode.
    You should pre-test it to ensure that it works on your CPU. */
void yy_cpu_measure_freq(void);

/** Returns CPU frequency in Hz.
    You should call yy_cpu_measure_freq() at least once before calling this
    function. */
u64 yy_cpu_get_freq(void);

/** Returns tick per second.
    You should call yy_cpu_measure_freq() at least once before calling this
    function. This function may used with yy_time_get_ticks() for benchmark. */
u64 yy_cpu_get_tick_per_sec(void);

/** Returns cpu cycle tick.
    You should call yy_cpu_measure_freq() at least once before calling this
    function. This function may used with yy_time_get_ticks() for benchmark. */
f64 yy_cpu_get_cycle_per_tick(void);



/*==============================================================================
 * Environment
 *============================================================================*/

/** Returns OS description. */
const char *yy_env_get_os_desc(void);

/** Returns CPU description. */
const char *yy_env_get_cpu_desc(void);

/** Returns compiler description. */
const char *yy_env_get_compiler_desc(void);



/*==============================================================================
 * Random Number Generator
 *============================================================================*/

/** Reset the random number generator with default seed. */
void yy_random_reset(void);

/** Generate a uniformly distributed 32-bit random number. */
u32 yy_random32(void);

/** Generate a uniformly distributed number, where 0 <= r < bound. */
u32 yy_random32_uniform(u32 bound);

/** Generate a uniformly distributed number, where min <= r <= max. */
u32 yy_random32_range(u32 min, u32 max);

/** Generate a uniformly distributed 64-bit random number. */
u64 yy_random64(void);

/** Generate a uniformly distributed number, where 0 <= r < bound. */
u64 yy_random64_uniform(u64 bound);

/** Generate a uniformly distributed number, where min <= r <= max. */
u64 yy_random64_range(u64 min, u64 max);



/*==============================================================================
 * File Utils
 *============================================================================*/

#ifdef _WIN32
#define YY_DIR_SEPARATOR '\\'
#else
#define YY_DIR_SEPARATOR '/'
#endif
#define YY_MAX_PATH 4096


/** Combine multiple component into a path, store the result to the buffer.
    The input parameter list should end with NULL.
    Return false if input is NULL. */
#if yy_has_attribute(sentinel)
__attribute__((sentinel))
#endif
bool yy_path_combine(char *buf, const char *path, ...);

/** Remove the last component of path, copy the result to the buffer.
    Return false if input is NULL or no last component. */
bool yy_path_remove_last(char *buf, const char *path);

/** Get the last component of path, copy it to the buffer.
    Return false if input is NULL or no last component. */
bool yy_path_get_last(char *buf, const char *path);

/** Append a file extension to the path, copy the result to the buffer.
    Return false if input is NULL. */
bool yy_path_append_ext(char *buf, const char *path, const char *ext);

/** Remove the file extension, copy the result to the buffer.
    Return false if input is NULL or no extension. */
bool yy_path_remove_ext(char *buf, const char *path);

/** Get the file extension, copy it to the buffer.
    Return false if input is NULL or no extension. */
bool yy_path_get_ext(char *buf, const char *path);



/** Returns whether a path exist. */
bool yy_path_exist(const char *path);

/** Returns whether a path is directory. */
bool yy_path_is_dir(const char *path);



/** Returns content file names of a dir. Returns NULL on error.
    The result should be released by yy_dir_free() */
char **yy_dir_read(const char *path, int *count);

/** Returns content file full paths of a dir. Returns NULL on error.
    The result should be released by yy_dir_free() */
char **yy_dir_read_full(const char *path, int *count);

/** Free the return value of yy_dir_read(). */
void yy_dir_free(char **names);



/** Read a file to memory, dat should be release with free(). */
bool yy_file_read(const char *path, u8 **dat, usize *len);

/** Read a file to memory with zero padding, dat should be release with free(). */
bool yy_file_read_with_padding(const char *path, u8 **dat, usize *len, usize padding);

/** Write data to file, overwrite if exist. */
bool yy_file_write(const char *path, u8 *dat, usize len);

/** Delete a file, returns true if success. */
bool yy_file_delete(const char *path);



/*==============================================================================
 * String Utils
 *============================================================================*/

/** Copy a string, same as strdup(). */
char *yy_str_copy(const char *str);

/** Returns whether the string contains a given string. */
bool yy_str_contains(const char *str, const char *search);

/** Returns whether the string begins with a prefix. */
bool yy_str_has_prefix(const char *str, const char *prefix);

/** Returns whether the string ends with a suffix. */
bool yy_str_has_suffix(const char *str, const char *suffix);



/*==============================================================================
 * Memory Buffer
 *============================================================================*/

/** A memory buffer s*/
typedef struct yy_buf {
    u8 *cur; /* cursor between hdr and end */
    u8 *hdr; /* head of the buffer */
    u8 *end; /* tail of the buffer */
    bool need_free;
} yy_buf;

/** Initialize a memory buffer with length. */
bool yy_buf_init(yy_buf *buf, usize len);

/** Release the memory in buffer. */
void yy_buf_release(yy_buf *buf);

/** Returns the used length of buffer (cur - hdr). */
usize yy_buf_len(yy_buf *buf);

/** Increase memory buffer and let (end - cur >= len). */
bool yy_buf_grow(yy_buf *buf, usize len);

/** Append data to buffer and move cursor. */
bool yy_buf_append(yy_buf *buf, u8 *dat, usize len);



/*==============================================================================
 * String Builder
 *============================================================================*/

/** A string builder */
typedef struct yy_buf yy_sb;

/** Initialize a string builder with capacity. */
bool yy_sb_init(yy_sb *buf, usize len);

/** Release the string builder. */
void yy_sb_release(yy_sb *buf);

/** Returns the length of string. */
usize yy_sb_get_len(yy_sb *sb);

/** Returns the inner string */
char *yy_sb_get_str(yy_sb *sb);

/** Copies and returns the string, should be released with free(). */
char *yy_sb_copy_str(yy_sb *sb, usize *len);

/** Append string. */
bool yy_sb_append(yy_sb *sb, const char *str);

/** Append string and escape html. */
bool yy_sb_append_html(yy_sb *sb, const char *str);

/** Append string and escape single character. */
bool yy_sb_append_esc(yy_sb *sb, char esc, const char *str);

/** Append string with format. */
bool yy_sb_printf(yy_sb *sb, const char *fmt, ...);



/*==============================================================================
 * Data Reader
 *============================================================================*/

/** A data reader */
typedef struct yy_buf yy_dat;

/** Initialize a data reader with file. */
bool yy_dat_init_with_file(yy_dat *dat, const char *path);

/** Initialize a data reader with memory (no copy). */
bool yy_dat_init_with_mem(yy_dat *dat, u8 *mem, usize len);

/** Release the data reader. */
void yy_dat_release(yy_dat *dat);

/** Reset the cursor of data reader. */
void yy_dat_reset(yy_dat *dat);

/** Read a line from data reader (NULL on end or error).
    The cursor will moved to next line.
    The string is not null-terminated. */
char *yy_dat_read_line(yy_dat *dat, usize *len);

/** Read and copy a line from data reader (NULL on end or error).
    The cursor will moved to next line.
    The return value should be release with free(). */
char *yy_dat_copy_line(yy_dat *dat, usize *len);



/*==============================================================================
 * Timer (Private)
 *============================================================================*/

#if defined(_WIN32)

struct yy_time {
    LARGE_INTEGER counter;
};

static yy_inline void yy_time_get_current(yy_time *t) {
    QueryPerformanceCounter(&t->counter);
}

static yy_inline f64 yy_time_to_seconds(yy_time *t) {
    LARGE_INTEGER freq;
    QueryPerformanceFrequency(&freq);
    return (f64)t->counter.QuadPart / (f64)freq.QuadPart;
}

#else

struct yy_time {
    struct timeval now;
};

static yy_inline void yy_time_get_current(yy_time *t) {
    gettimeofday(&t->now, NULL);
}

static yy_inline f64 yy_time_to_seconds(yy_time *t) {
    return (f64)t->now.tv_sec + (f64)t->now.tv_usec / 1000.0 / 1000.0;
}

#endif

static yy_inline f64 yy_time_get_seconds(void) {
    yy_time t;
    yy_time_get_current(&t);
    return yy_time_to_seconds(&t);
}

#if defined(_WIN32) && (defined(_M_IX86) || defined(_M_AMD64))
#    pragma intrinsic(__rdtsc)
#endif

static yy_inline u64 yy_time_get_ticks() {
    /*
     RDTSC is a fixed-frequency timer on modern x86 CPU,
     and may not match to CPU clock cycles.
     */
#if defined(_WIN32)
#   if defined(_M_IX86) || defined(_M_AMD64)
    return __rdtsc();
#   else
    LARGE_INTEGER now;
    QueryPerformanceCounter(&now);
    return (u64)now.QuadPart;
#   endif
    
#elif defined(__i386__) || defined(__i386)
    u64 tsc;
    __asm volatile("rdtsc" : "=a"(tsc));
    return tsc;
    
#elif defined(__x86_64__) || defined(__x86_64) || \
defined(__amd64__) || defined(__amd64)
    u64 lo, hi;
    __asm volatile("rdtsc" : "=a"(lo), "=d"(hi));
    return (hi << 32u) | lo;
    
#elif defined(__ia64__)
    u64 tsc;
    __asm volatile("mov %0 = ar.itc" : "=r"(tsc));
    return tsc;
    
    /*
     We can use several counter registers in the ARM CPU:
     
     CNTPCT_EL0: physical counter
     CNTVCT_EL0: virtual counter (physical counter - offset)
     PMCCNTR_EL0: performance monitors control cycle count register
     
     The physical counter (and virtual counter) runs at a fixed frequency which
     is different with the CPU cycle rate. For example: Apple A10 max clock rate
     is 2.34GHz, but the physical counter frequency is fixed to 24MHz.
     
     Some of these registers are optional, or may be disabled in user mode.
     For example: read CNTVCT_EL0 or PMCCNTR_EL0 in iPhone may get
     'EXC_BAD_INSTRUCTION' exception.
     */
#elif defined(__aarch64__)
    u64 tsc;
#   if defined(__APPLE__)
    /* used by mach_absolute_time(), see mach_absolute_time.s */
    __asm volatile("mrs %0, cntpct_el0" : "=r"(tsc));
#   else
    __asm volatile("mrs %0, cntvct_el0" : "=r"(tsc));
#   endif
    return tsc;
    
#elif defined(__ARM_ARCH) && defined(__APPLE__)
    return mach_absolute_time();
    
#else
    struct timeval now;
    gettimeofday(&now, NULL);
    return (u64)now.tv_sec * 1000000 + now.tv_usec;
#endif
}



#ifdef __cplusplus
}
#endif

#endif /* yy_test_utils_h */

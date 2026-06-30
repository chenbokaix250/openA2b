#ifndef A2B_LOGGER_H
#define A2B_LOGGER_H

#include "a2b_types.h"
#include "a2b_error.h"

/* Log levels */
typedef enum {
    A2B_LOG_NONE = 0,
    A2B_LOG_ERROR,
    A2B_LOG_WARN,
    A2B_LOG_INFO,
    A2B_LOG_DEBUG,
    A2B_LOG_TRACE
} A2B_LogLevel;

/* Log tag - component identifier */
typedef const char *A2B_LogTag;

#define A2B_LOG_TAG_MAIN       "MAIN"
#define A2B_LOG_TAG_DISCOVERY  "DISCOVERY"
#define A2B_LOG_TAG_NETWORK    "NETWORK"
#define A2B_LOG_TAG_ROUTE      "ROUTE"
#define A2B_LOG_TAG_CLOCK      "CLOCK"
#define A2B_LOG_TAG_IRQ        "IRQ"
#define A2B_LOG_TAG_I2C        "I2C"
#define A2B_LOG_TAG_REG        "REG"
#define A2B_LOG_TAG_STATE      "STATE"

/* Compile-time log level control */
#ifndef A2B_LOG_LEVEL
#define A2B_LOG_LEVEL A2B_LOG_INFO
#endif

/* Log macros - zero overhead when disabled */
#if A2B_LOG_LEVEL == A2B_LOG_NONE
#define A2B_LOG(level, tag, fmt, ...)   ((void)0)
#else
#define A2B_LOG(level, tag, fmt, ...) \
    a2b_logger_log(level, tag, __FILE__, __LINE__, fmt, ##__VA_ARGS__)
#endif

#define A2B_LOG_ERROR(tag, fmt, ...)   A2B_LOG(A2B_LOG_ERROR, tag, fmt, ##__VA_ARGS__)
#define A2B_LOG_WARN(tag, fmt, ...)    A2B_LOG(A2B_LOG_WARN,  tag, fmt, ##__VA_ARGS__)
#define A2B_LOG_INFO(tag, fmt, ...)    A2B_LOG(A2B_LOG_INFO,  tag, fmt, ##__VA_ARGS__)
#define A2B_LOG_DEBUG(tag, fmt, ...)   A2B_LOG(A2B_LOG_DEBUG, tag, fmt, ##__VA_ARGS__)
#define A2B_LOG_TRACE(tag, fmt, ...)    A2B_LOG(A2B_LOG_TRACE, tag, fmt, ##__VA_ARGS__)

/**
 * Initialize logger
 */
void A2B_LoggerInit(void);

/**
 * Set minimum log level
 */
void A2B_LoggerSetLevel(A2B_LogLevel level);

/**
 * Get current log level
 */
A2B_LogLevel A2B_LoggerGetLevel(void);

/**
 * Core logging function
 */
void a2b_logger_log(A2B_LogLevel level, A2B_LogTag tag,
                    const char *file, int line,
                    const char *fmt, ...);

#endif /* A2B_LOGGER_H */

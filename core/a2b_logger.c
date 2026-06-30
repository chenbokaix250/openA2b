#include "a2b_logger.h"
#include <stdio.h>
#include <stdarg.h>

static A2B_LogLevel g_log_level = A2B_LOG_INFO;

static const char *level_to_string(A2B_LogLevel level)
{
    switch (level) {
        case A2B_LOG_ERROR: return "ERROR";
        case A2B_LOG_WARN:  return "WARN ";
        case A2B_LOG_INFO:  return "INFO ";
        case A2B_LOG_DEBUG: return "DEBUG";
        case A2B_LOG_TRACE: return "TRACE";
        default:             return "     ";
    }
}

void A2B_LoggerInit(void)
{
    g_log_level = A2B_LOG_INFO;
}

void A2B_LoggerSetLevel(A2B_LogLevel level)
{
    g_log_level = level;
}

A2B_LogLevel A2B_LoggerGetLevel(void)
{
    return g_log_level;
}

void a2b_logger_log(A2B_LogLevel level, A2B_LogTag tag,
                    const char *file, int line,
                    const char *fmt, ...)
{
    if (level > g_log_level) {
        return;
    }

    /* Extract basename from file path */
    const char *basename = file;
    const char *p = file;
    while (*p) {
        if (*p == '/' && p[1]) {
            basename = p + 1;
        }
        p++;
    }

    printf("[%s][%-8s] ", level_to_string(level), tag);
    printf("%s:%d: ", basename, line);

    va_list args;
    va_start(args, fmt);
    vprintf(fmt, args);
    va_end(args);

    printf("\n");
}

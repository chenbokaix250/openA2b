#ifndef A2B_TIMER_H
#define A2B_TIMER_H

#include "a2b_types.h"
#include "a2b_error.h"

/**
 * Timer callback function type
 */
typedef void (*A2B_TimerCallback)(void *priv);

/**
 * Timer operations interface
 */
typedef struct _A2B_TimerOps {
    A2B_Result (*init)(A2B_TIMER_HANDLE handle, void *config);
    A2B_Result (*deinit)(A2B_TIMER_HANDLE handle);
    A2B_Result (*start)(A2B_TIMER_HANDLE handle, uint32_t delay_ms);
    A2B_Result (*stop)(A2B_TIMER_HANDLE handle);
    bool       (*is_running)(A2B_TIMER_HANDLE handle);
} A2B_TimerOps;

/**
 * Timer handle structure
 */
struct _A2B_Timer_Ops {
    const A2B_TimerOps *ops;
    void *priv;
};

/**
 * Initialize timer HAL
 */
A2B_TIMER_HANDLE A2B_TimerInit(void);

/**
 * Close timer HAL
 */
A2B_Result A2B_TimerDeinit(A2B_TIMER_HANDLE handle);

/**
 * Start a one-shot timer
 */
A2B_Result A2B_TimerStart(A2B_TIMER_HANDLE handle, uint32_t delay_ms,
                            A2B_TimerCallback callback, void *priv);

/**
 * Stop a running timer
 */
A2B_Result A2B_TimerStop(A2B_TIMER_HANDLE handle);

/**
 * Check if timer is running
 */
bool A2B_TimerIsRunning(A2B_TIMER_HANDLE handle);

#endif /* A2B_TIMER_H */

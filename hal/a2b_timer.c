#include "a2b_timer.h"

typedef struct _A2B_Timer_StubData {
    bool initialized;
    bool running;
} A2B_Timer_StubData;

static A2B_Timer_StubData g_timer_data = { false, false };

A2B_TIMER_HANDLE A2B_TimerInit(void)
{
    g_timer_data.initialized = true;
    g_timer_data.running = false;
    return (A2B_TIMER_HANDLE)&g_timer_data;
}

A2B_Result A2B_TimerDeinit(A2B_TIMER_HANDLE handle)
{
    (void)handle;
    g_timer_data.initialized = false;
    g_timer_data.running = false;
    return A2B_SUCCESS;
}

A2B_Result A2B_TimerStart(A2B_TIMER_HANDLE handle, uint32_t delay_ms,
                            A2B_TimerCallback callback, void *priv)
{
    (void)handle; (void)delay_ms; (void)callback; (void)priv;
    g_timer_data.running = true;
    return A2B_ERR_UNSUPPORTED;  /* Stub: callback not fired */
}

A2B_Result A2B_TimerStop(A2B_TIMER_HANDLE handle)
{
    (void)handle;
    g_timer_data.running = false;
    return A2B_SUCCESS;
}

bool A2B_TimerIsRunning(A2B_TIMER_HANDLE handle)
{
    (void)handle;
    return g_timer_data.running;
}

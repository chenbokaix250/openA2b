#include "unity.h"
#include "a2b_timer.h"

static bool g_callback_fired = false;
static void *g_callback_priv = NULL;

void timer_callback(void *priv)
{
    g_callback_fired = true;
    g_callback_priv = priv;
}

void test_A2B_TimerInit_returns_non_null(void)
{
    A2B_TIMER_HANDLE handle = A2B_TimerInit();
    TEST_ASSERT_NOT_NULL(handle);
}

void test_A2B_TimerDeinit_returns_success(void)
{
    A2B_TIMER_HANDLE handle = A2B_TimerInit();
    A2B_Result result = A2B_TimerDeinit(handle);
    TEST_ASSERT_EQUAL_INT32(A2B_SUCCESS, result);
}

void test_A2B_TimerStop_returns_success(void)
{
    A2B_TIMER_HANDLE handle = A2B_TimerInit();
    A2B_Result result = A2B_TimerStop(handle);
    TEST_ASSERT_EQUAL_INT32(A2B_SUCCESS, result);
}

void test_A2B_TimerIsRunning_returns_false_after_init(void)
{
    A2B_TIMER_HANDLE handle = A2B_TimerInit();
    bool running = A2B_TimerIsRunning(handle);
    TEST_ASSERT_FALSE(running);
}

#include "unity.h"
#include "a2b_state.h"

static A2B_State g_last_old_state = A2B_STATE_INIT;
static A2B_State g_last_new_state = A2B_STATE_INIT;
static bool g_callback_fired = false;

void state_callback(A2B_State old_state, A2B_State new_state, void *priv)
{
    (void)priv;
    g_last_old_state = old_state;
    g_last_new_state = new_state;
    g_callback_fired = true;
}

static void reset_globals(void)
{
    g_last_old_state = A2B_STATE_INIT;
    g_last_new_state = A2B_STATE_INIT;
    g_callback_fired = false;
}

void test_A2B_StateMachineInit_sets_init_state(void)
{
    A2B_StateMachine sm;
    A2B_StateMachineInit(&sm, NULL, NULL);

    TEST_ASSERT_EQUAL_INT(A2B_STATE_INIT, A2B_StateMachineGetState(&sm));
}

void test_A2B_StateMachineInit_with_callback(void)
{
    A2B_StateMachine sm;
    A2B_StateMachineInit(&sm, state_callback, NULL);

    TEST_ASSERT_EQUAL_INT(A2B_STATE_INIT, sm.current_state);
    TEST_ASSERT_NOT_NULL(sm.callback);
}

void test_A2B_StateMachineTransition_INIT_to_IDLE(void)
{
    A2B_StateMachine sm;
    A2B_StateMachineInit(&sm, NULL, NULL);

    A2B_Result res = A2B_StateMachineTransition(&sm, A2B_STATE_IDLE);

    TEST_ASSERT_EQUAL_INT32(A2B_SUCCESS, res);
    TEST_ASSERT_EQUAL_INT(A2B_STATE_IDLE, A2B_StateMachineGetState(&sm));
}

void test_A2B_StateMachineTransition_fires_callback(void)
{
    reset_globals();
    A2B_StateMachine sm;
    A2B_StateMachineInit(&sm, state_callback, NULL);

    A2B_StateMachineTransition(&sm, A2B_STATE_IDLE);

    TEST_ASSERT_TRUE(g_callback_fired);
    TEST_ASSERT_EQUAL_INT(A2B_STATE_INIT, g_last_old_state);
    TEST_ASSERT_EQUAL_INT(A2B_STATE_IDLE, g_last_new_state);
}

void test_A2B_StateMachineTransition_invalid_returns_error(void)
{
    A2B_StateMachine sm;
    A2B_StateMachineInit(&sm, NULL, NULL);

    /* Can't go directly from INIT to RUNNING */
    A2B_Result res = A2B_StateMachineTransition(&sm, A2B_STATE_RUNNING);

    TEST_ASSERT_EQUAL_INT32(A2B_ERR_STATE_INVALID, res);
    TEST_ASSERT_EQUAL_INT(A2B_STATE_INIT, A2B_StateMachineGetState(&sm));
}

void test_A2B_StateMachineIsValidTransition(void)
{
    TEST_ASSERT_TRUE(A2B_StateMachineIsValidTransition(A2B_STATE_INIT, A2B_STATE_IDLE));
    TEST_ASSERT_TRUE(A2B_StateMachineIsValidTransition(A2B_STATE_IDLE, A2B_STATE_WAIT_POWER));
    TEST_ASSERT_TRUE(A2B_StateMachineIsValidTransition(A2B_STATE_RUNNING, A2B_STATE_LINK_DOWN));
    TEST_ASSERT_FALSE(A2B_StateMachineIsValidTransition(A2B_STATE_INIT, A2B_STATE_RUNNING));
    TEST_ASSERT_FALSE(A2B_StateMachineIsValidTransition(A2B_STATE_SHUTDOWN, A2B_STATE_INIT));
}

void test_A2B_StateMachineTransition_full_sequence(void)
{
    A2B_StateMachine sm;
    A2B_StateMachineInit(&sm, NULL, NULL);

    /* INIT -> IDLE */
    TEST_ASSERT_EQUAL_INT32(A2B_SUCCESS,
        A2B_StateMachineTransition(&sm, A2B_STATE_IDLE));

    /* IDLE -> WAIT_POWER */
    TEST_ASSERT_EQUAL_INT32(A2B_SUCCESS,
        A2B_StateMachineTransition(&sm, A2B_STATE_WAIT_POWER));

    /* WAIT_POWER -> WAIT_PLL */
    TEST_ASSERT_EQUAL_INT32(A2B_SUCCESS,
        A2B_StateMachineTransition(&sm, A2B_STATE_WAIT_PLL));

    /* WAIT_PLL -> DISCOVERY */
    TEST_ASSERT_EQUAL_INT32(A2B_SUCCESS,
        A2B_StateMachineTransition(&sm, A2B_STATE_DISCOVERY));

    /* DISCOVERY -> CONFIG_NODE */
    TEST_ASSERT_EQUAL_INT32(A2B_SUCCESS,
        A2B_StateMachineTransition(&sm, A2B_STATE_CONFIG_NODE));
}

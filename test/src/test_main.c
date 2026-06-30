#include "unity.h"
#include "mock_i2c.h"

void setUp(void)
{
    MockI2C_Init();
}

void tearDown(void)
{
    MockI2C_Reset();
}

extern void test_A2B_MAX_NODES_is_16(void);
extern void test_A2B_MAX_ROUTES_is_64(void);
extern void test_A2B_Config_default(void);
extern void test_A2B_State_enum_values(void);
extern void test_A2B_SUCCESS_is_zero(void);
extern void test_A2B_ERR_I2C_NACK_is_negative(void);
extern void test_A2B_ERR_NODE_NOT_FOUND_is_negative(void);
extern void test_A2B_ResultToString_returns_success_for_zero(void);
extern void test_A2B_ResultToString_returns_error_for_negative(void);
extern void test_A2B_I2CInit_returns_non_null(void);
extern void test_A2B_I2CInit_twice_does_not_crash(void);
extern void test_A2B_I2CDeinit_returns_success(void);
extern void test_A2B_I2CWrite_returns_unsupported_on_stub(void);
extern void test_A2B_I2CRead_returns_unsupported_on_stub(void);
extern void test_MockI2C_Init_succeeds(void);
extern void test_MockI2C_InjectNackOnWrite(void);
extern void test_MockI2C_InjectNackOnRead(void);
extern void test_MockI2C_GetResponse_returns_valid_addr(void);
extern void test_A2B_TimerInit_returns_non_null(void);
extern void test_A2B_TimerDeinit_returns_success(void);
extern void test_A2B_TimerStop_returns_success(void);
extern void test_A2B_TimerIsRunning_returns_false_after_init(void);

int main(void)
{
    UNITY_BEGIN();

    /* Types */
    RUN_TEST(test_A2B_MAX_NODES_is_16);
    RUN_TEST(test_A2B_MAX_ROUTES_is_64);
    RUN_TEST(test_A2B_Config_default);
    RUN_TEST(test_A2B_State_enum_values);

    /* Error */
    RUN_TEST(test_A2B_SUCCESS_is_zero);
    RUN_TEST(test_A2B_ERR_I2C_NACK_is_negative);
    RUN_TEST(test_A2B_ERR_NODE_NOT_FOUND_is_negative);
    RUN_TEST(test_A2B_ResultToString_returns_success_for_zero);
    RUN_TEST(test_A2B_ResultToString_returns_error_for_negative);

    /* I2C HAL */
    RUN_TEST(test_A2B_I2CInit_returns_non_null);
    RUN_TEST(test_A2B_I2CInit_twice_does_not_crash);
    RUN_TEST(test_A2B_I2CDeinit_returns_success);
    RUN_TEST(test_A2B_I2CWrite_returns_unsupported_on_stub);
    RUN_TEST(test_A2B_I2CRead_returns_unsupported_on_stub);

    /* Mock I2C */
    RUN_TEST(test_MockI2C_Init_succeeds);
    RUN_TEST(test_MockI2C_InjectNackOnWrite);
    RUN_TEST(test_MockI2C_InjectNackOnRead);
    RUN_TEST(test_MockI2C_GetResponse_returns_valid_addr);

    /* Timer */
    RUN_TEST(test_A2B_TimerInit_returns_non_null);
    RUN_TEST(test_A2B_TimerDeinit_returns_success);
    RUN_TEST(test_A2B_TimerStop_returns_success);
    RUN_TEST(test_A2B_TimerIsRunning_returns_false_after_init);

    return UNITY_END();
}
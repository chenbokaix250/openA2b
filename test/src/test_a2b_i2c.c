#include "unity.h"
#include "a2b_i2c.h"

void test_A2B_I2CInit_returns_non_null(void)
{
    A2B_I2C_HANDLE handle = A2B_I2CInit();
    TEST_ASSERT_NOT_NULL(handle);
}

void test_A2B_I2CInit_twice_does_not_crash(void)
{
    A2B_I2C_HANDLE h1 = A2B_I2CInit();
    A2B_I2C_HANDLE h2 = A2B_I2CInit();
    TEST_ASSERT_NOT_NULL(h1);
    TEST_ASSERT_NOT_NULL(h2);
}

void test_A2B_I2CDeinit_returns_success(void)
{
    A2B_I2C_HANDLE handle = A2B_I2CInit();
    A2B_Result result = A2B_I2CDeinit(handle);
    TEST_ASSERT_EQUAL_INT32(A2B_SUCCESS, result);
}

void test_A2B_I2CWrite_returns_unsupported_on_stub(void)
{
    A2B_I2C_HANDLE handle = A2B_I2CInit();
    uint8_t data[4] = { 0 };
    A2B_Result result = A2B_I2CWrite(handle, 0x68, data, sizeof(data));
    TEST_ASSERT_EQUAL_INT32(A2B_ERR_UNSUPPORTED, result);
}

void test_A2B_I2CRead_returns_unsupported_on_stub(void)
{
    A2B_I2C_HANDLE handle = A2B_I2CInit();
    uint8_t data[4] = { 0 };
    A2B_Result result = A2B_I2CRead(handle, 0x68, data, sizeof(data));
    TEST_ASSERT_EQUAL_INT32(A2B_ERR_UNSUPPORTED, result);
}
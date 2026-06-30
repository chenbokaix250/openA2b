#include "unity.h"
#include "mock_ad2428.h"

void test_MockAD2428_Init(void)
{
    MockAD2428 dev;
    MockAD2428_Init(&dev, 0x68);

    TEST_ASSERT_EQUAL_UINT8(0x68, dev.i2c_addr);
}

void test_MockAD2428_Reset_sets_defaults(void)
{
    MockAD2428 dev;
    MockAD2428_Init(&dev, 0x68);
    MockAD2428_SetReg(&dev, 0x03, 0xFF);  /* Set control */

    MockAD2428_Reset(&dev);

    TEST_ASSERT_EQUAL_UINT8(0x28, MockAD2428_GetReg(&dev, 0x00));  /* chip_id */
    TEST_ASSERT_EQUAL_UINT8(0x00, MockAD2428_GetReg(&dev, 0x03));  /* control */
}

void test_MockAD2428_SetReg_and_GetReg(void)
{
    MockAD2428 dev;
    MockAD2428_Init(&dev, 0x68);

    MockAD2428_SetReg(&dev, 0x03, 0x42);

    TEST_ASSERT_EQUAL_UINT8(0x42, MockAD2428_GetReg(&dev, 0x03));
}

void test_MockAD2428_I2CWrite(void)
{
    MockAD2428 dev;
    MockAD2428_Init(&dev, 0x68);

    uint8_t data[1] = { 0x42 };
    A2B_Result res = MockAD2428_I2CWrite(&dev, 0x03, data, 1);

    TEST_ASSERT_EQUAL_INT32(A2B_SUCCESS, res);
    TEST_ASSERT_EQUAL_UINT8(0x42, MockAD2428_GetReg(&dev, 0x03));
}

void test_MockAD2428_I2CRead(void)
{
    MockAD2428 dev;
    MockAD2428_Init(&dev, 0x68);
    MockAD2428_SetReg(&dev, 0x00, 0x28);

    uint8_t data[1] = { 0 };
    A2B_Result res = MockAD2428_I2CRead(&dev, 0x00, data, 1);

    TEST_ASSERT_EQUAL_INT32(A2B_SUCCESS, res);
    TEST_ASSERT_EQUAL_UINT8(0x28, data[0]);
}

void test_MockAD2428_InjectNack(void)
{
    MockAD2428 dev;
    MockAD2428_Init(&dev, 0x68);
    MockAD2428_InjectNack(&dev);

    uint8_t data[1] = { 0x42 };
    A2B_Result res = MockAD2428_I2CWrite(&dev, 0x03, data, 1);

    TEST_ASSERT_EQUAL_INT32(A2B_ERR_I2C_NACK, res);
    /* NACK should be consumed */
    res = MockAD2428_I2CWrite(&dev, 0x03, data, 1);
    TEST_ASSERT_EQUAL_INT32(A2B_SUCCESS, res);
}

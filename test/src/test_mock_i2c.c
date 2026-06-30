#include "unity.h"
#include "mock_i2c.h"

void test_MockI2C_Init_succeeds(void)
{
    /* MockI2C_Init() called in setUp - just verify no crash */
}

void test_MockI2C_InjectNackOnWrite(void)
{
    MockI2C_InjectNackOnWrite(0x68);
    const MockI2C_Response *resp = MockI2C_GetResponse(0x68);
    TEST_ASSERT_TRUE(resp->nack_on_write);
}

void test_MockI2C_InjectNackOnRead(void)
{
    MockI2C_InjectNackOnRead(0x68);
    const MockI2C_Response *resp = MockI2C_GetResponse(0x68);
    TEST_ASSERT_TRUE(resp->nack_on_read);
}

void test_MockI2C_GetResponse_returns_valid_addr(void)
{
    const MockI2C_Response *resp = MockI2C_GetResponse(0x68);
    TEST_ASSERT_EQUAL_UINT8(0x68, resp->addr);
}
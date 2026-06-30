#include "unity.h"
#include "a2b_error.h"

void setUp(void) {}
void tearDown(void) {}

void test_A2B_SUCCESS_is_zero(void)
{
    TEST_ASSERT_EQUAL_INT32(0, A2B_SUCCESS);
}

void test_A2B_ERR_I2C_NACK_is_negative(void)
{
    TEST_ASSERT_TRUE(A2B_ERR_I2C_NACK < 0);
}

void test_A2B_ERR_NODE_NOT_FOUND_is_negative(void)
{
    TEST_ASSERT_TRUE(A2B_ERR_NODE_NOT_FOUND < 0);
}

void test_A2B_ResultToString_returns_success_for_zero(void)
{
    const char *str = A2B_ResultToString(A2B_SUCCESS);
    TEST_ASSERT_NOT_NULL(str);
    TEST_ASSERT_EQUAL_STRING("SUCCESS", str);
}

void test_A2B_ResultToString_returns_error_for_negative(void)
{
    const char *str = A2B_ResultToString(A2B_ERR_I2C_NACK);
    TEST_ASSERT_NOT_NULL(str);
    TEST_ASSERT_TRUE(str[0] == 'E' || str[0] == 'e');
}

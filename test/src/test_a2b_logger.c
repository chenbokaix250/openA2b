#include "unity.h"
#include "a2b_logger.h"

void test_A2B_LoggerInit_succeeds(void)
{
    A2B_LoggerInit();  /* Should not crash */
    TEST_PASS();
}

void test_A2B_LoggerSetLevel_and_GetLevel(void)
{
    A2B_LoggerSetLevel(A2B_LOG_DEBUG);
    TEST_ASSERT_EQUAL_INT(A2B_LOG_DEBUG, A2B_LoggerGetLevel());

    A2B_LoggerSetLevel(A2B_LOG_ERROR);
    TEST_ASSERT_EQUAL_INT(A2B_LOG_ERROR, A2B_LoggerGetLevel());
}

void test_A2B_LOG_ERROR_macro_compiles(void)
{
    /* Macro should compile without error */
    A2B_LOG_ERROR(A2B_LOG_TAG_I2C, "Test error message");
    TEST_PASS();
}

void test_A2B_LOG_INFO_macro_compiles(void)
{
    /* Macro should compile without error */
    A2B_LOG_INFO(A2B_LOG_TAG_MAIN, "Test info message");
    TEST_PASS();
}

void test_A2B_LOG_LEVEL_none_disables_logging(void)
{
    /* When compiled with A2B_LOG_LEVEL=A2B_LOG_NONE, A2B_LOG expands to ((void)0) */
    /* This test just verifies compilation - output is discarded */
    A2B_LOG(A2B_LOG_TRACE, A2B_LOG_TAG_MAIN, "Should be discarded");
    TEST_PASS();
}

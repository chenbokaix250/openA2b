#include "unity.h"
#include "a2b_clock.h"
#include "a2b_device.h"

void test_A2B_ClockMgrInit(void)
{
    A2B_Device dev;
    A2B_ClockMgr mgr;
    A2B_Config config = A2B_CONFIG_DEFAULT;

    A2B_DeviceInit(&dev, &config);
    A2B_ClockMgrInit(&mgr, &dev);

    TEST_ASSERT_EQUAL_INT(A2B_PLL_UNLOCKED, A2B_ClockMgrGetPLLStatus(&mgr));

    A2B_DeviceDeinit(&dev);
}

void test_A2B_ClockMgrConfigure(void)
{
    A2B_Device dev;
    A2B_ClockMgr mgr;
    A2B_Config config = A2B_CONFIG_DEFAULT;
    A2B_ClockConfig clock_config = {
        .source = A2B_CLOCK_SRC_LOCAL,
        .target_freq_hz = 48000000,
        .pll_multiplier = 4
    };

    A2B_DeviceInit(&dev, &config);
    A2B_ClockMgrInit(&mgr, &dev);

    A2B_Result res = A2B_ClockMgrConfigure(&mgr, &clock_config);

    TEST_ASSERT_EQUAL_INT32(A2B_SUCCESS, res);

    A2B_DeviceDeinit(&dev);
}

void test_A2B_ClockMgrStartPLL(void)
{
    A2B_Device dev;
    A2B_ClockMgr mgr;
    A2B_Config config = A2B_CONFIG_DEFAULT;

    A2B_DeviceInit(&dev, &config);
    A2B_ClockMgrInit(&mgr, &dev);

    A2B_Result res = A2B_ClockMgrStartPLL(&mgr);

    TEST_ASSERT_EQUAL_INT32(A2B_SUCCESS, res);
    TEST_ASSERT_EQUAL_INT(A2B_PLL_LOCKING, A2B_ClockMgrGetPLLStatus(&mgr));

    A2B_DeviceDeinit(&dev);
}

void test_A2B_ClockMgrWaitLock(void)
{
    A2B_Device dev;
    A2B_ClockMgr mgr;
    A2B_Config config = A2B_CONFIG_DEFAULT;

    A2B_DeviceInit(&dev, &config);
    A2B_ClockMgrInit(&mgr, &dev);

    A2B_ClockMgrStartPLL(&mgr);

    A2B_Result res = A2B_ClockMgrWaitLock(&mgr, 1000);

    TEST_ASSERT_EQUAL_INT32(A2B_SUCCESS, res);
    TEST_ASSERT_EQUAL_INT(A2B_PLL_LOCKED, A2B_ClockMgrGetPLLStatus(&mgr));

    A2B_DeviceDeinit(&dev);
}

void test_A2B_ClockMgrGetPLLStatus(void)
{
    A2B_Device dev;
    A2B_ClockMgr mgr;
    A2B_Config config = A2B_CONFIG_DEFAULT;

    A2B_DeviceInit(&dev, &config);
    A2B_ClockMgrInit(&mgr, &dev);

    TEST_ASSERT_EQUAL_INT(A2B_PLL_UNLOCKED, A2B_ClockMgrGetPLLStatus(&mgr));

    A2B_ClockMgrStartPLL(&mgr);
    TEST_ASSERT_EQUAL_INT(A2B_PLL_LOCKING, A2B_ClockMgrGetPLLStatus(&mgr));

    A2B_DeviceDeinit(&dev);
}

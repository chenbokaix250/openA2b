#include "unity.h"
#include "a2b_device.h"

void test_A2B_DeviceInit_with_valid_config(void)
{
    A2B_Device dev;
    A2B_Config config = A2B_CONFIG_DEFAULT;

    A2B_Result res = A2B_DeviceInit(&dev, &config);

    TEST_ASSERT_EQUAL_INT32(A2B_SUCCESS, res);
    TEST_ASSERT_NOT_NULL(dev.bus);
    TEST_ASSERT_EQUAL_INT(A2B_STATE_INIT, dev.state);
    TEST_ASSERT_EQUAL_UINT8(0, dev.node_count);
}

void test_A2B_DeviceInit_with_null_device(void)
{
    A2B_Config config = A2B_CONFIG_DEFAULT;

    A2B_Result res = A2B_DeviceInit(NULL, &config);

    TEST_ASSERT_EQUAL_INT32(A2B_ERR_INVALID_PARAM, res);
}

void test_A2B_DeviceInit_with_null_config(void)
{
    A2B_Device dev;

    A2B_Result res = A2B_DeviceInit(&dev, NULL);

    TEST_ASSERT_EQUAL_INT32(A2B_ERR_INVALID_PARAM, res);
}

void test_A2B_DeviceDeinit(void)
{
    A2B_Device dev;
    A2B_Config config = A2B_CONFIG_DEFAULT;
    A2B_DeviceInit(&dev, &config);

    A2B_Result res = A2B_DeviceDeinit(&dev);

    TEST_ASSERT_EQUAL_INT32(A2B_SUCCESS, res);
    TEST_ASSERT_EQUAL_INT(A2B_STATE_SHUTDOWN, dev.state);
}

void test_A2B_DeviceGetState(void)
{
    A2B_Device dev;
    A2B_Config config = A2B_CONFIG_DEFAULT;
    A2B_DeviceInit(&dev, &config);

    A2B_State state = A2B_DeviceGetState(&dev);

    TEST_ASSERT_EQUAL_INT(A2B_STATE_INIT, state);
}

void test_A2B_DeviceSetState(void)
{
    A2B_Device dev;
    A2B_Config config = A2B_CONFIG_DEFAULT;
    A2B_DeviceInit(&dev, &config);

    A2B_Result res = A2B_DeviceSetState(&dev, A2B_STATE_IDLE);

    TEST_ASSERT_EQUAL_INT32(A2B_SUCCESS, res);
    TEST_ASSERT_EQUAL_INT(A2B_STATE_IDLE, A2B_DeviceGetState(&dev));
}

void test_A2B_DeviceInit_sets_max_nodes_limit(void)
{
    A2B_Device dev;
    A2B_Config config = { 0x68, 400000, 20 };  /* Request 20 nodes */

    A2B_DeviceInit(&dev, &config);

    /* Should be capped to A2B_MAX_NODES (16) */
    TEST_ASSERT_EQUAL_UINT8(A2B_MAX_NODES, dev.config.max_nodes);
}

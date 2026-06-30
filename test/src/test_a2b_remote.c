#include "unity.h"
#include "a2b_remote.h"
#include "a2b_device.h"

void test_A2B_RemoteRegInit(void)
{
    A2B_Device dev;
    A2B_Config config = A2B_CONFIG_DEFAULT;
    A2B_DeviceInit(&dev, &config);

    A2B_RemoteReg ctx;
    A2B_RemoteRegInit(&ctx, &dev);

    TEST_ASSERT_EQUAL_PTR(&dev, ctx.dev);

    A2B_DeviceDeinit(&dev);
}

void test_A2B_RemoteRegInit_with_null_context(void)
{
    A2B_Device dev;
    A2B_Config config = A2B_CONFIG_DEFAULT;
    A2B_DeviceInit(&dev, &config);

    A2B_Result res = A2B_RemoteRegInit(NULL, &dev);

    TEST_ASSERT_EQUAL_INT32(A2B_ERR_INVALID_PARAM, res);

    A2B_DeviceDeinit(&dev);
}

void test_A2B_RemoteRegInit_with_null_device(void)
{
    A2B_RemoteReg ctx;

    A2B_Result res = A2B_RemoteRegInit(&ctx, NULL);

    TEST_ASSERT_EQUAL_INT32(A2B_ERR_INVALID_PARAM, res);
}

void test_A2B_RemoteRegRead_returns_unsupported(void)
{
    A2B_Device dev;
    A2B_Config config = A2B_CONFIG_DEFAULT;
    A2B_DeviceInit(&dev, &config);

    A2B_RemoteReg ctx;
    A2B_RemoteRegInit(&ctx, &dev);

    uint32_t val = 0;
    A2B_Result res = A2B_RemoteRegRead(&ctx, 0, 0x00, &val);

    TEST_ASSERT_EQUAL_INT32(A2B_ERR_UNSUPPORTED, res);

    A2B_DeviceDeinit(&dev);
}

void test_A2B_RemoteRegWrite_returns_unsupported(void)
{
    A2B_Device dev;
    A2B_Config config = A2B_CONFIG_DEFAULT;
    A2B_DeviceInit(&dev, &config);

    A2B_RemoteReg ctx;
    A2B_RemoteRegInit(&ctx, &dev);

    A2B_Result res = A2B_RemoteRegWrite(&ctx, 0, 0x00, 0x1234);

    TEST_ASSERT_EQUAL_INT32(A2B_ERR_UNSUPPORTED, res);

    A2B_DeviceDeinit(&dev);
}

void test_A2B_RemoteRegUpdateBits_returns_unsupported(void)
{
    A2B_Device dev;
    A2B_Config config = A2B_CONFIG_DEFAULT;
    A2B_DeviceInit(&dev, &config);

    A2B_RemoteReg ctx;
    A2B_RemoteRegInit(&ctx, &dev);

    A2B_Result res = A2B_RemoteRegUpdateBits(&ctx, 0, 0x00, 0xFF, 0xAA);

    TEST_ASSERT_EQUAL_INT32(A2B_ERR_UNSUPPORTED, res);

    A2B_DeviceDeinit(&dev);
}

#include "unity.h"
#include "a2b_audio.h"
#include "a2b_device.h"
#include "a2b_network.h"

void test_A2B_AudioMgrInit(void)
{
    A2B_Device dev;
    A2B_NetworkMgr net_mgr;
    A2B_AudioMgr audio_mgr;
    A2B_Config config = A2B_CONFIG_DEFAULT;

    A2B_DeviceInit(&dev, &config);
    A2B_NetworkMgrInit(&net_mgr, &dev);
    A2B_AudioMgrInit(&audio_mgr, &dev, &net_mgr, 4);

    TEST_ASSERT_EQUAL_UINT8(0, A2B_AudioMgrGetRouteCount(&audio_mgr));

    A2B_DeviceDeinit(&dev);
}

void test_A2B_AudioMgrSetRoute(void)
{
    A2B_Device dev;
    A2B_NetworkMgr net_mgr;
    A2B_AudioMgr audio_mgr;
    A2B_AudioRoute routes[4];
    A2B_Config config = A2B_CONFIG_DEFAULT;

    A2B_DeviceInit(&dev, &config);
    A2B_NetworkMgrInit(&net_mgr, &dev);
    A2B_AudioMgrInit(&audio_mgr, &dev, &net_mgr, 4);
    audio_mgr.routes = routes;

    A2B_AudioRoute route = {
        .src_node = 0,
        .dst_node = 1,
        .src_slot = 0,
        .dst_slot = 0,
        .num_slots = 2,
        .direction = A2B_AUDIO_DIR_DOWNSTREAM,
        .format = A2B_AUDIO_FMT_I2S
    };

    A2B_Result res = A2B_AudioMgrSetRoute(&audio_mgr, &route);

    TEST_ASSERT_EQUAL_INT32(A2B_SUCCESS, res);
    TEST_ASSERT_EQUAL_UINT8(1, A2B_AudioMgrGetRouteCount(&audio_mgr));

    A2B_DeviceDeinit(&dev);
}

void test_A2B_AudioMgrEnableRoute(void)
{
    A2B_Device dev;
    A2B_NetworkMgr net_mgr;
    A2B_AudioMgr audio_mgr;
    A2B_AudioRoute routes[4];
    A2B_Config config = A2B_CONFIG_DEFAULT;

    A2B_DeviceInit(&dev, &config);
    A2B_NetworkMgrInit(&net_mgr, &dev);
    A2B_AudioMgrInit(&audio_mgr, &dev, &net_mgr, 4);
    audio_mgr.routes = routes;

    A2B_AudioRoute route = {
        .src_node = 0, .dst_node = 1, .direction = A2B_AUDIO_DIR_DOWNSTREAM
    };
    A2B_AudioMgrSetRoute(&audio_mgr, &route);

    A2B_Result res = A2B_AudioMgrEnableRoute(&audio_mgr, 0);

    TEST_ASSERT_EQUAL_INT32(A2B_SUCCESS, res);
    TEST_ASSERT_TRUE(routes[0].enabled);

    A2B_DeviceDeinit(&dev);
}

void test_A2B_AudioMgrDisableRoute(void)
{
    A2B_Device dev;
    A2B_NetworkMgr net_mgr;
    A2B_AudioMgr audio_mgr;
    A2B_AudioRoute routes[4];
    A2B_Config config = A2B_CONFIG_DEFAULT;

    A2B_DeviceInit(&dev, &config);
    A2B_NetworkMgrInit(&net_mgr, &dev);
    A2B_AudioMgrInit(&audio_mgr, &dev, &net_mgr, 4);
    audio_mgr.routes = routes;

    A2B_AudioRoute route = {
        .src_node = 0, .dst_node = 1, .direction = A2B_AUDIO_DIR_DOWNSTREAM
    };
    A2B_AudioMgrSetRoute(&audio_mgr, &route);
    A2B_AudioMgrEnableRoute(&audio_mgr, 0);

    A2B_Result res = A2B_AudioMgrDisableRoute(&audio_mgr, 0);

    TEST_ASSERT_EQUAL_INT32(A2B_SUCCESS, res);
    TEST_ASSERT_FALSE(routes[0].enabled);

    A2B_DeviceDeinit(&dev);
}

void test_A2B_AudioMgrSetRoute_multiple_routes(void)
{
    A2B_Device dev;
    A2B_NetworkMgr net_mgr;
    A2B_AudioMgr audio_mgr;
    A2B_AudioRoute routes[4];
    A2B_Config config = A2B_CONFIG_DEFAULT;

    A2B_DeviceInit(&dev, &config);
    A2B_NetworkMgrInit(&net_mgr, &dev);
    A2B_AudioMgrInit(&audio_mgr, &dev, &net_mgr, 4);
    audio_mgr.routes = routes;

    A2B_AudioRoute route1 = { .src_node = 0, .dst_node = 1 };
    A2B_AudioRoute route2 = { .src_node = 1, .dst_node = 2 };

    A2B_AudioMgrSetRoute(&audio_mgr, &route1);
    A2B_AudioMgrSetRoute(&audio_mgr, &route2);

    TEST_ASSERT_EQUAL_UINT8(2, A2B_AudioMgrGetRouteCount(&audio_mgr));

    A2B_DeviceDeinit(&dev);
}

#include "unity.h"
#include "a2b_discovery.h"
#include "a2b_device.h"
#include "a2b_network.h"

static A2B_Device       g_dev;
static A2B_NetworkMgr   g_net_mgr;
static A2B_DiscoveryMgr  g_discovery_mgr;

/* Initialization for tests - called from test_main.c's setUp */
void A2B_DiscoveryTestInit(void)
{
    A2B_Config config = A2B_CONFIG_DEFAULT;
    A2B_DeviceInit(&g_dev, &config);
    A2B_NetworkMgrInit(&g_net_mgr, &g_dev);
    A2B_DiscoveryMgrInit(&g_discovery_mgr, &g_dev, &g_net_mgr);
}

/* Cleanup for tests - called from test_main.c's tearDown */
void A2B_DiscoveryTestDeinit(void)
{
    A2B_DeviceDeinit(&g_dev);
}

void test_A2B_DiscoveryMgrInit(void)
{
    TEST_ASSERT_EQUAL_INT(A2B_DISC_STATUS_NOT_STARTED,
                         A2B_DiscoveryGetStatus(&g_discovery_mgr));
}

void test_A2B_DiscoveryStart(void)
{
    A2B_Result res = A2B_DiscoveryStart(&g_discovery_mgr);

    TEST_ASSERT_EQUAL_INT32(A2B_SUCCESS, res);
    TEST_ASSERT_EQUAL_INT(A2B_DISC_STATUS_IN_PROGRESS,
                         A2B_DiscoveryGetStatus(&g_discovery_mgr));
}

void test_A2B_DiscoveryStep(void)
{
    A2B_DiscoveryStart(&g_discovery_mgr);

    A2B_Result res = A2B_DiscoveryStep(&g_discovery_mgr);

    TEST_ASSERT_EQUAL_INT32(A2B_SUCCESS, res);
    TEST_ASSERT_EQUAL_UINT8(1, A2B_NetworkMgrGetNodeCount(&g_net_mgr));
}

void test_A2B_DiscoveryStep_multiple_nodes(void)
{
    A2B_DiscoveryStart(&g_discovery_mgr);

    /* Discover 3 nodes */
    A2B_DiscoveryStep(&g_discovery_mgr);
    A2B_DiscoveryStep(&g_discovery_mgr);
    A2B_DiscoveryStep(&g_discovery_mgr);

    TEST_ASSERT_EQUAL_UINT8(3, A2B_NetworkMgrGetNodeCount(&g_net_mgr));
}

void test_A2B_DiscoveryComplete(void)
{
    A2B_DiscoveryStart(&g_discovery_mgr);

    A2B_Result res = A2B_DiscoveryComplete(&g_discovery_mgr);

    TEST_ASSERT_EQUAL_INT32(A2B_SUCCESS, res);
    TEST_ASSERT_EQUAL_INT(A2B_DISC_STATUS_SUCCESS,
                         A2B_DiscoveryGetStatus(&g_discovery_mgr));
}

void test_A2B_DiscoveryAbort(void)
{
    A2B_DiscoveryStart(&g_discovery_mgr);

    A2B_Result res = A2B_DiscoveryAbort(&g_discovery_mgr);

    TEST_ASSERT_EQUAL_INT32(A2B_SUCCESS, res);
    TEST_ASSERT_EQUAL_INT(A2B_DISC_STATUS_FAILED,
                         A2B_DiscoveryGetStatus(&g_discovery_mgr));
}

void test_A2B_DiscoveryStart_when_already_in_progress(void)
{
    A2B_DiscoveryStart(&g_discovery_mgr);

    /* Should return SUCCESS (idempotent) */
    A2B_Result res = A2B_DiscoveryStart(&g_discovery_mgr);

    TEST_ASSERT_EQUAL_INT32(A2B_SUCCESS, res);
}

void test_A2B_DiscoveryStep_when_not_started(void)
{
    /* Discovery not started */
    A2B_Result res = A2B_DiscoveryStep(&g_discovery_mgr);

    TEST_ASSERT_EQUAL_INT32(A2B_ERR_STATE_INVALID, res);
}

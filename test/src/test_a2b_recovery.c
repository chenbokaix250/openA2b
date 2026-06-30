#include "unity.h"
#include "a2b_recovery.h"
#include "a2b_device.h"
#include "a2b_network.h"
#include "a2b_diagnostic.h"

static A2B_Device    g_dev;
static A2B_NetworkMgr g_net_mgr;
static A2B_DiagMgr   g_diag_mgr;
static A2B_RecoveryMgr g_recovery_mgr;
static A2B_DiagRecord g_records[8];

/* Initialization for tests - called from test_main.c's setUp */
void A2B_RecoveryTestInit(void)
{
    A2B_Config config = A2B_CONFIG_DEFAULT;
    A2B_DeviceInit(&g_dev, &config);
    A2B_NetworkMgrInit(&g_net_mgr, &g_dev);
    A2B_DiagMgrInit(&g_diag_mgr, &g_dev, &g_net_mgr, 8);
    g_diag_mgr.records = g_records;
    A2B_RecoveryMgrInit(&g_recovery_mgr, &g_dev, &g_net_mgr, &g_diag_mgr);
}

/* Cleanup for tests - called from test_main.c's tearDown */
void A2B_RecoveryTestDeinit(void)
{
    A2B_DeviceDeinit(&g_dev);
}

void test_A2B_RecoveryMgrInit(void)
{
    TEST_ASSERT_EQUAL_INT(A2B_RECOVERY_IDLE, A2B_RecoveryMgrGetStatus(&g_recovery_mgr));
}

void test_A2B_RecoveryMgrRecover_NODE_RESET(void)
{
    A2B_Result res = A2B_RecoveryMgrRecover(&g_recovery_mgr,
                                              A2B_RECOVERY_NODE_RESET, 1);

    TEST_ASSERT_EQUAL_INT32(A2B_SUCCESS, res);
    TEST_ASSERT_EQUAL_INT(A2B_RECOVERY_SUCCESS, A2B_RecoveryMgrGetStatus(&g_recovery_mgr));
}

void test_A2B_RecoveryMgrRecover_LINK_RENEGOTIATE(void)
{
    A2B_Result res = A2B_RecoveryMgrRecover(&g_recovery_mgr,
                                              A2B_RECOVERY_LINK_RENEGOTIATE, 2);

    TEST_ASSERT_EQUAL_INT32(A2B_SUCCESS, res);
}

void test_A2B_RecoveryMgrRecover_FULL_DISCOVERY(void)
{
    A2B_Result res = A2B_RecoveryMgrRecover(&g_recovery_mgr,
                                              A2B_RECOVERY_FULL_DISCOVERY, 0);

    TEST_ASSERT_EQUAL_INT32(A2B_SUCCESS, res);
}

void test_A2B_RecoveryMgrAbort(void)
{
    A2B_RecoveryMgrRecover(&g_recovery_mgr, A2B_RECOVERY_NODE_RESET, 1);

    A2B_Result res = A2B_RecoveryMgrAbort(&g_recovery_mgr);

    TEST_ASSERT_EQUAL_INT32(A2B_SUCCESS, res);
    TEST_ASSERT_EQUAL_INT(A2B_RECOVERY_IDLE, A2B_RecoveryMgrGetStatus(&g_recovery_mgr));
}

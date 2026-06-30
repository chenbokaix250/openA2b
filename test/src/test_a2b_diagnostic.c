#include "unity.h"
#include "a2b_diagnostic.h"
#include "a2b_device.h"
#include "a2b_network.h"

static A2B_Device    g_dev;
static A2B_NetworkMgr g_net_mgr;
static A2B_DiagMgr   g_diag_mgr;
static A2B_DiagRecord g_records[8];

/* Initialization for tests - called from test_main.c's setUp */
void A2B_DiagTestInit(void)
{
    A2B_Config config = A2B_CONFIG_DEFAULT;
    A2B_DeviceInit(&g_dev, &config);
    A2B_NetworkMgrInit(&g_net_mgr, &g_dev);
    A2B_DiagMgrInit(&g_diag_mgr, &g_dev, &g_net_mgr, 8);
    g_diag_mgr.records = g_records;
}

/* Cleanup for tests - called from test_main.c's tearDown */
void A2B_DiagTestDeinit(void)
{
    A2B_DeviceDeinit(&g_dev);
}

void test_A2B_DiagMgrInit(void)
{
    TEST_ASSERT_EQUAL_UINT16(0, A2B_DiagMgrGetRecordCount(&g_diag_mgr));
}

void test_A2B_DiagMgrLogEvent(void)
{
    A2B_DiagMgrLogEvent(&g_diag_mgr, A2B_DIAG_EVENT_I2C_ERROR, 0, 0x42);

    TEST_ASSERT_EQUAL_UINT16(1, A2B_DiagMgrGetRecordCount(&g_diag_mgr));
}

void test_A2B_DiagMgrGetLatestRecord(void)
{
    A2B_DiagMgrLogEvent(&g_diag_mgr, A2B_DIAG_EVENT_CRC_ERROR, 1, 0x10);
    A2B_DiagMgrLogEvent(&g_diag_mgr, A2B_DIAG_EVENT_FRAME_ERROR, 2, 0x20);

    A2B_DiagRecord rec = { 0 };
    A2B_Result res = A2B_DiagMgrGetLatestRecord(&g_diag_mgr, &rec);

    TEST_ASSERT_EQUAL_INT32(A2B_SUCCESS, res);
    TEST_ASSERT_EQUAL_INT(A2B_DIAG_EVENT_FRAME_ERROR, rec.event);
    TEST_ASSERT_EQUAL_UINT8(2, rec.node_id);
}

void test_A2B_DiagMgrClearRecords(void)
{
    A2B_DiagMgrLogEvent(&g_diag_mgr, A2B_DIAG_EVENT_I2C_ERROR, 0, 0);
    A2B_DiagMgrLogEvent(&g_diag_mgr, A2B_DIAG_EVENT_I2C_ERROR, 1, 0);

    A2B_DiagMgrClearRecords(&g_diag_mgr);

    TEST_ASSERT_EQUAL_UINT16(0, A2B_DiagMgrGetRecordCount(&g_diag_mgr));
}

void test_A2B_DiagMgrLogEvent_wraps_around(void)
{
    /* Log 10 events into 8-slot buffer */
    for (int i = 0; i < 10; i++) {
        A2B_DiagMgrLogEvent(&g_diag_mgr, A2B_DIAG_EVENT_I2C_ERROR, i, 0);
    }

    /* Should have 8 records (max capacity) */
    TEST_ASSERT_EQUAL_UINT16(8, A2B_DiagMgrGetRecordCount(&g_diag_mgr));
}
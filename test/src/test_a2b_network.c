#include "unity.h"
#include "a2b_network.h"
#include "a2b_device.h"

void test_A2B_NetworkMgrInit(void)
{
    A2B_Device dev;
    A2B_NetworkMgr mgr;
    A2B_Config config = A2B_CONFIG_DEFAULT;

    A2B_DeviceInit(&dev, &config);
    A2B_NetworkMgrInit(&mgr, &dev);

    TEST_ASSERT_EQUAL_PTR(dev.nodes, mgr.nodes);
    TEST_ASSERT_EQUAL_UINT8(0, mgr.node_count);

    A2B_DeviceDeinit(&dev);
}

void test_A2B_NetworkMgrAddNode(void)
{
    A2B_Device dev;
    A2B_NetworkMgr mgr;
    A2B_Config config = A2B_CONFIG_DEFAULT;
    A2B_DeviceInit(&dev, &config);
    A2B_NetworkMgrInit(&mgr, &dev);

    A2B_NodeInfo node = {
        .id = 0,
        .address = 0x68,
        .vendor_id = 0x1234,
        .product_id = 0xABCD,
        .state = A2B_NODE_STATE_ONLINE
    };

    A2B_Result res = A2B_NetworkMgrAddNode(&mgr, &node);

    TEST_ASSERT_EQUAL_INT32(A2B_SUCCESS, res);
    TEST_ASSERT_EQUAL_UINT8(1, A2B_NetworkMgrGetNodeCount(&mgr));

    A2B_DeviceDeinit(&dev);
}

void test_A2B_NetworkMgrAddNode_assigns_id(void)
{
    A2B_Device dev;
    A2B_NetworkMgr mgr;
    A2B_Config config = A2B_CONFIG_DEFAULT;
    A2B_DeviceInit(&dev, &config);
    A2B_NetworkMgrInit(&mgr, &dev);

    A2B_NodeInfo node = {
        .address = 0x68,
        .vendor_id = 0x1234,
        .product_id = 0xABCD
    };

    A2B_NetworkMgrAddNode(&mgr, &node);

    TEST_ASSERT_EQUAL_UINT8(0, mgr.nodes[0].id);

    A2B_DeviceDeinit(&dev);
}

void test_A2B_NetworkMgrFindNodeById(void)
{
    A2B_Device dev;
    A2B_NetworkMgr mgr;
    A2B_Config config = A2B_CONFIG_DEFAULT;
    A2B_DeviceInit(&dev, &config);
    A2B_NetworkMgrInit(&mgr, &dev);

    A2B_NodeInfo node = { .address = 0x68, .vendor_id = 0x1234 };
    A2B_NetworkMgrAddNode(&mgr, &node);

    A2B_NodeInfo *found = NULL;
    A2B_Result res = A2B_NetworkMgrFindNodeById(&mgr, 0, &found);

    TEST_ASSERT_EQUAL_INT32(A2B_SUCCESS, res);
    TEST_ASSERT_NOT_NULL(found);
    TEST_ASSERT_EQUAL_UINT8(0x68, found->address);

    A2B_DeviceDeinit(&dev);
}

void test_A2B_NetworkMgrFindNodeById_not_found(void)
{
    A2B_Device dev;
    A2B_NetworkMgr mgr;
    A2B_Config config = A2B_CONFIG_DEFAULT;
    A2B_DeviceInit(&dev, &config);
    A2B_NetworkMgrInit(&mgr, &dev);

    A2B_NodeInfo *found = NULL;
    A2B_Result res = A2B_NetworkMgrFindNodeById(&mgr, 99, &found);

    TEST_ASSERT_EQUAL_INT32(A2B_ERR_NODE_NOT_FOUND, res);

    A2B_DeviceDeinit(&dev);
}

void test_A2B_NetworkMgrFindNodeByAddr(void)
{
    A2B_Device dev;
    A2B_NetworkMgr mgr;
    A2B_Config config = A2B_CONFIG_DEFAULT;
    A2B_DeviceInit(&dev, &config);
    A2B_NetworkMgrInit(&mgr, &dev);

    A2B_NodeInfo node = { .address = 0x68, .vendor_id = 0x1234 };
    A2B_NetworkMgrAddNode(&mgr, &node);

    A2B_NodeInfo *found = NULL;
    A2B_Result res = A2B_NetworkMgrFindNodeByAddr(&mgr, 0x68, &found);

    TEST_ASSERT_EQUAL_INT32(A2B_SUCCESS, res);
    TEST_ASSERT_EQUAL_UINT8(0, found->id);

    A2B_DeviceDeinit(&dev);
}

void test_A2B_NetworkMgrRemoveNode(void)
{
    A2B_Device dev;
    A2B_NetworkMgr mgr;
    A2B_Config config = A2B_CONFIG_DEFAULT;
    A2B_DeviceInit(&dev, &config);
    A2B_NetworkMgrInit(&mgr, &dev);

    A2B_NodeInfo node1 = { .address = 0x68 };
    A2B_NodeInfo node2 = { .address = 0x69 };
    A2B_NetworkMgrAddNode(&mgr, &node1);
    A2B_NetworkMgrAddNode(&mgr, &node2);

    A2B_Result res = A2B_NetworkMgrRemoveNode(&mgr, 0);

    TEST_ASSERT_EQUAL_INT32(A2B_SUCCESS, res);
    TEST_ASSERT_EQUAL_UINT8(1, A2B_NetworkMgrGetNodeCount(&mgr));
    TEST_ASSERT_EQUAL_UINT8(0x69, mgr.nodes[0].address);

    A2B_DeviceDeinit(&dev);
}

void test_A2B_NetworkMgrClear(void)
{
    A2B_Device dev;
    A2B_NetworkMgr mgr;
    A2B_Config config = A2B_CONFIG_DEFAULT;
    A2B_DeviceInit(&dev, &config);
    A2B_NetworkMgrInit(&mgr, &dev);

    A2B_NodeInfo node = { .address = 0x68 };
    A2B_NetworkMgrAddNode(&mgr, &node);
    A2B_NetworkMgrAddNode(&mgr, &node);

    A2B_NetworkMgrClear(&mgr);

    TEST_ASSERT_EQUAL_UINT8(0, A2B_NetworkMgrGetNodeCount(&mgr));

    A2B_DeviceDeinit(&dev);
}
#include "unity.h"
#include "a2b_slot.h"
#include "a2b_device.h"

void test_A2B_SlotMgrInit(void)
{
    A2B_Device dev;
    A2B_SlotMgr slot_mgr;
    A2B_SlotConfig slots[8];
    A2B_Config config = A2B_CONFIG_DEFAULT;

    A2B_DeviceInit(&dev, &config);
    A2B_SlotMgrInit(&slot_mgr, &dev, 8);
    slot_mgr.slots = slots;

    TEST_ASSERT_EQUAL_UINT8(0, A2B_SlotMgrGetSlotCount(&slot_mgr));

    A2B_DeviceDeinit(&dev);
}

void test_A2B_SlotMgrSetSlot(void)
{
    A2B_Device dev;
    A2B_SlotMgr slot_mgr;
    A2B_SlotConfig slots[8];
    A2B_Config config = A2B_CONFIG_DEFAULT;

    A2B_DeviceInit(&dev, &config);
    A2B_SlotMgrInit(&slot_mgr, &dev, 8);
    slot_mgr.slots = slots;

    A2B_SlotConfig slot = {
        .slot_index = 0,
        .slot_width = 16,
        .left_justified = true,
        .inverted_clock = false,
        .delay_bits = 0
    };

    A2B_Result res = A2B_SlotMgrSetSlot(&slot_mgr, &slot);

    TEST_ASSERT_EQUAL_INT32(A2B_SUCCESS, res);
    TEST_ASSERT_EQUAL_UINT8(1, A2B_SlotMgrGetSlotCount(&slot_mgr));

    A2B_DeviceDeinit(&dev);
}

void test_A2B_SlotMgrGetSlot(void)
{
    A2B_Device dev;
    A2B_SlotMgr slot_mgr;
    A2B_SlotConfig slots[8];
    A2B_Config config = A2B_CONFIG_DEFAULT;

    A2B_DeviceInit(&dev, &config);
    A2B_SlotMgrInit(&slot_mgr, &dev, 8);
    slot_mgr.slots = slots;

    A2B_SlotConfig slot = {
        .slot_index = 2,
        .slot_width = 24,
        .left_justified = false
    };
    A2B_SlotMgrSetSlot(&slot_mgr, &slot);

    A2B_SlotConfig retrieved = { 0 };
    A2B_Result res = A2B_SlotMgrGetSlot(&slot_mgr, 2, &retrieved);

    TEST_ASSERT_EQUAL_INT32(A2B_SUCCESS, res);
    TEST_ASSERT_EQUAL_UINT8(24, retrieved.slot_width);
    TEST_ASSERT_FALSE(retrieved.left_justified);

    A2B_DeviceDeinit(&dev);
}

void test_A2B_SlotMgrSetSlot_out_of_range(void)
{
    A2B_Device dev;
    A2B_SlotMgr slot_mgr;
    A2B_SlotConfig slots[8];
    A2B_Config config = A2B_CONFIG_DEFAULT;

    A2B_DeviceInit(&dev, &config);
    A2B_SlotMgrInit(&slot_mgr, &dev, 8);
    slot_mgr.slots = slots;

    A2B_SlotConfig slot = {
        .slot_index = 99,  /* Out of range */
        .slot_width = 16
    };

    A2B_Result res = A2B_SlotMgrSetSlot(&slot_mgr, &slot);

    TEST_ASSERT_EQUAL_INT32(A2B_ERR_INVALID_PARAM, res);

    A2B_DeviceDeinit(&dev);
}

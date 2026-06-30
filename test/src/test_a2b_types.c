#include "unity.h"
#include "a2b_types.h"

void setUp(void) {}
void tearDown(void) {}

void test_A2B_MAX_NODES_is_16(void)
{
    TEST_ASSERT_EQUAL_UINT8(16, A2B_MAX_NODES);
}

void test_A2B_MAX_ROUTES_is_64(void)
{
    TEST_ASSERT_EQUAL_UINT8(64, A2B_MAX_ROUTES);
}

void test_A2B_Config_default(void)
{
    A2B_Config cfg = A2B_CONFIG_DEFAULT;
    TEST_ASSERT_EQUAL_UINT8(0x68, cfg.master_address);
    TEST_ASSERT_EQUAL_UINT32(400000, cfg.bus_speed_hz);
    TEST_ASSERT_EQUAL_UINT8(8, cfg.max_nodes);
}

void test_A2B_State_enum_values(void)
{
    TEST_ASSERT_EQUAL_INT(0, A2B_STATE_INIT);
    TEST_ASSERT_EQUAL_INT(1, A2B_STATE_IDLE);
    TEST_ASSERT_EQUAL_INT(2, A2B_STATE_ERROR);
    TEST_ASSERT_EQUAL_INT(3, A2B_STATE_SHUTDOWN);
}

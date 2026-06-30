#include "unity.h"
#include "mock_i2c.h"
#include "a2b_logger.h"
#include "ad2428_reg.h"
#include "a2b_device.h"
#include "a2b_state.h"
#include "a2b_network.h"
#include "a2b_discovery.h"
#include "a2b_irq.h"
#include "a2b_remote.h"

/* Register test reset function */
extern void AD2428_RegTestReset(void);

/* Discovery test initialization/cleanup */
extern void A2B_DiscoveryTestInit(void);
extern void A2B_DiscoveryTestDeinit(void);

void setUp(void)
{
    MockI2C_Init();
    A2B_LoggerInit();
    AD2428_RegTestReset();
    A2B_DiscoveryTestInit();
}

void tearDown(void)
{
    A2B_DiscoveryTestDeinit();
    MockI2C_Reset();
}

extern void test_A2B_MAX_NODES_is_16(void);
extern void test_A2B_MAX_ROUTES_is_64(void);
extern void test_A2B_Config_default(void);
extern void test_A2B_State_enum_values(void);
extern void test_A2B_SUCCESS_is_zero(void);
extern void test_A2B_ERR_I2C_NACK_is_negative(void);
extern void test_A2B_ERR_NODE_NOT_FOUND_is_negative(void);
extern void test_A2B_ResultToString_returns_success_for_zero(void);
extern void test_A2B_ResultToString_returns_error_for_negative(void);
extern void test_A2B_I2CInit_returns_non_null(void);
extern void test_A2B_I2CInit_twice_does_not_crash(void);
extern void test_A2B_I2CDeinit_returns_success(void);
extern void test_A2B_I2CWrite_returns_unsupported_on_stub(void);
extern void test_A2B_I2CRead_returns_unsupported_on_stub(void);
extern void test_MockI2C_Init_succeeds(void);
extern void test_MockI2C_InjectNackOnWrite(void);
extern void test_MockI2C_InjectNackOnRead(void);
extern void test_MockI2C_GetResponse_returns_valid_addr(void);
extern void test_A2B_TimerInit_returns_non_null(void);
extern void test_A2B_TimerDeinit_returns_success(void);
extern void test_A2B_TimerStop_returns_success(void);
extern void test_A2B_TimerIsRunning_returns_false_after_init(void);
extern void test_A2B_LoggerInit_succeeds(void);
extern void test_A2B_LoggerSetLevel_and_GetLevel(void);
extern void test_A2B_LOG_ERROR_macro_compiles(void);
extern void test_A2B_LOG_INFO_macro_compiles(void);
extern void test_A2B_LOG_LEVEL_none_disables_logging(void);
extern void test_AD2428_RegInit_sets_context(void);
extern void test_AD2428_RegRead_calls_read_fn(void);
extern void test_AD2428_RegWrite_calls_write_fn(void);
extern void test_AD2428_RegUpdateBits_reads_modifies_writes(void);
extern void test_A2B_DeviceInit_with_valid_config(void);
extern void test_A2B_DeviceInit_with_null_device(void);
extern void test_A2B_DeviceInit_with_null_config(void);
extern void test_A2B_DeviceDeinit(void);
extern void test_A2B_DeviceGetState(void);
extern void test_A2B_DeviceSetState(void);
extern void test_A2B_DeviceInit_sets_max_nodes_limit(void);
extern void test_A2B_StateMachineInit_sets_init_state(void);
extern void test_A2B_StateMachineInit_with_callback(void);
extern void test_A2B_StateMachineTransition_INIT_to_IDLE(void);
extern void test_A2B_StateMachineTransition_fires_callback(void);
extern void test_A2B_StateMachineTransition_invalid_returns_error(void);
extern void test_A2B_StateMachineIsValidTransition(void);
extern void test_A2B_StateMachineTransition_full_sequence(void);
extern void test_A2B_NetworkMgrInit(void);
extern void test_A2B_NetworkMgrAddNode(void);
extern void test_A2B_NetworkMgrAddNode_assigns_id(void);
extern void test_A2B_NetworkMgrFindNodeById(void);
extern void test_A2B_NetworkMgrFindNodeById_not_found(void);
extern void test_A2B_NetworkMgrFindNodeByAddr(void);
extern void test_A2B_NetworkMgrRemoveNode(void);
extern void test_A2B_NetworkMgrClear(void);
extern void test_A2B_DiscoveryMgrInit(void);
extern void test_A2B_DiscoveryStart(void);
extern void test_A2B_DiscoveryStep(void);
extern void test_A2B_DiscoveryStep_multiple_nodes(void);
extern void test_A2B_DiscoveryComplete(void);
extern void test_A2B_DiscoveryAbort(void);
extern void test_A2B_DiscoveryStart_when_already_in_progress(void);
extern void test_A2B_DiscoveryStep_when_not_started(void);

/* IRQ */
extern void test_A2B_IrqMgrInit(void);
extern void test_A2B_IrqMgrEnable(void);
extern void test_A2B_IrqMgrDisable(void);
extern void test_A2B_IrqMgrRegisterCallback(void);
extern void test_A2B_IrqMgrTriggerEvent_calls_callback(void);
extern void test_A2B_IrqMgrTriggerEvent_no_callback_when_disabled(void);

/* Remote Register */
extern void test_A2B_RemoteRegInit(void);
extern void test_A2B_RemoteRegInit_with_null_context(void);
extern void test_A2B_RemoteRegInit_with_null_device(void);
extern void test_A2B_RemoteRegRead_returns_unsupported(void);
extern void test_A2B_RemoteRegWrite_returns_unsupported(void);
extern void test_A2B_RemoteRegUpdateBits_returns_unsupported(void);

int main(void)
{
    UNITY_BEGIN();

    /* Types */
    RUN_TEST(test_A2B_MAX_NODES_is_16);
    RUN_TEST(test_A2B_MAX_ROUTES_is_64);
    RUN_TEST(test_A2B_Config_default);
    RUN_TEST(test_A2B_State_enum_values);

    /* Error */
    RUN_TEST(test_A2B_SUCCESS_is_zero);
    RUN_TEST(test_A2B_ERR_I2C_NACK_is_negative);
    RUN_TEST(test_A2B_ERR_NODE_NOT_FOUND_is_negative);
    RUN_TEST(test_A2B_ResultToString_returns_success_for_zero);
    RUN_TEST(test_A2B_ResultToString_returns_error_for_negative);

    /* I2C HAL */
    RUN_TEST(test_A2B_I2CInit_returns_non_null);
    RUN_TEST(test_A2B_I2CInit_twice_does_not_crash);
    RUN_TEST(test_A2B_I2CDeinit_returns_success);
    RUN_TEST(test_A2B_I2CWrite_returns_unsupported_on_stub);
    RUN_TEST(test_A2B_I2CRead_returns_unsupported_on_stub);

    /* Mock I2C */
    RUN_TEST(test_MockI2C_Init_succeeds);
    RUN_TEST(test_MockI2C_InjectNackOnWrite);
    RUN_TEST(test_MockI2C_InjectNackOnRead);
    RUN_TEST(test_MockI2C_GetResponse_returns_valid_addr);

    /* Timer */
    RUN_TEST(test_A2B_TimerInit_returns_non_null);
    RUN_TEST(test_A2B_TimerDeinit_returns_success);
    RUN_TEST(test_A2B_TimerStop_returns_success);
    RUN_TEST(test_A2B_TimerIsRunning_returns_false_after_init);

    /* Logger */
    RUN_TEST(test_A2B_LoggerInit_succeeds);
    RUN_TEST(test_A2B_LoggerSetLevel_and_GetLevel);
    RUN_TEST(test_A2B_LOG_ERROR_macro_compiles);
    RUN_TEST(test_A2B_LOG_INFO_macro_compiles);
    RUN_TEST(test_A2B_LOG_LEVEL_none_disables_logging);

    /* Register */
    RUN_TEST(test_AD2428_RegInit_sets_context);
    RUN_TEST(test_AD2428_RegRead_calls_read_fn);
    RUN_TEST(test_AD2428_RegWrite_calls_write_fn);
    RUN_TEST(test_AD2428_RegUpdateBits_reads_modifies_writes);

    /* Device */
    RUN_TEST(test_A2B_DeviceInit_with_valid_config);
    RUN_TEST(test_A2B_DeviceInit_with_null_device);
    RUN_TEST(test_A2B_DeviceInit_with_null_config);
    RUN_TEST(test_A2B_DeviceDeinit);
    RUN_TEST(test_A2B_DeviceGetState);
    RUN_TEST(test_A2B_DeviceSetState);
    RUN_TEST(test_A2B_DeviceInit_sets_max_nodes_limit);

    /* State Machine */
    RUN_TEST(test_A2B_StateMachineInit_sets_init_state);
    RUN_TEST(test_A2B_StateMachineInit_with_callback);
    RUN_TEST(test_A2B_StateMachineTransition_INIT_to_IDLE);
    RUN_TEST(test_A2B_StateMachineTransition_fires_callback);
    RUN_TEST(test_A2B_StateMachineTransition_invalid_returns_error);
    RUN_TEST(test_A2B_StateMachineIsValidTransition);
    RUN_TEST(test_A2B_StateMachineTransition_full_sequence);

    /* Network Manager */
    RUN_TEST(test_A2B_NetworkMgrInit);
    RUN_TEST(test_A2B_NetworkMgrAddNode);
    RUN_TEST(test_A2B_NetworkMgrAddNode_assigns_id);
    RUN_TEST(test_A2B_NetworkMgrFindNodeById);
    RUN_TEST(test_A2B_NetworkMgrFindNodeById_not_found);
    RUN_TEST(test_A2B_NetworkMgrFindNodeByAddr);
    RUN_TEST(test_A2B_NetworkMgrRemoveNode);
    RUN_TEST(test_A2B_NetworkMgrClear);

    /* Discovery */
    RUN_TEST(test_A2B_DiscoveryMgrInit);
    RUN_TEST(test_A2B_DiscoveryStart);
    RUN_TEST(test_A2B_DiscoveryStep);
    RUN_TEST(test_A2B_DiscoveryStep_multiple_nodes);
    RUN_TEST(test_A2B_DiscoveryComplete);
    RUN_TEST(test_A2B_DiscoveryAbort);
    RUN_TEST(test_A2B_DiscoveryStart_when_already_in_progress);
    RUN_TEST(test_A2B_DiscoveryStep_when_not_started);

    /* IRQ */
    RUN_TEST(test_A2B_IrqMgrInit);
    RUN_TEST(test_A2B_IrqMgrEnable);
    RUN_TEST(test_A2B_IrqMgrDisable);
    RUN_TEST(test_A2B_IrqMgrRegisterCallback);
    RUN_TEST(test_A2B_IrqMgrTriggerEvent_calls_callback);
    RUN_TEST(test_A2B_IrqMgrTriggerEvent_no_callback_when_disabled);

    /* Remote Register */
    RUN_TEST(test_A2B_RemoteRegInit);
    RUN_TEST(test_A2B_RemoteRegInit_with_null_context);
    RUN_TEST(test_A2B_RemoteRegInit_with_null_device);
    RUN_TEST(test_A2B_RemoteRegRead_returns_unsupported);
    RUN_TEST(test_A2B_RemoteRegWrite_returns_unsupported);
    RUN_TEST(test_A2B_RemoteRegUpdateBits_returns_unsupported);

    return UNITY_END();
}
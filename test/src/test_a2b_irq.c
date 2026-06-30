#include "unity.h"
#include "a2b_irq.h"
#include "a2b_device.h"

static A2B_IrqEvent g_last_irq_event = A2B_IRQ_EVENT_NONE;
static void *g_last_irq_priv = NULL;

static void irq_test_callback(A2B_IrqEvent event, void *priv)
{
    g_last_irq_event = event;
    g_last_irq_priv = priv;
}

void test_A2B_IrqMgrInit(void)
{
    A2B_Device dev;
    A2B_Config config = A2B_CONFIG_DEFAULT;
    A2B_DeviceInit(&dev, &config);

    A2B_IrqMgr mgr;
    A2B_IrqMgrInit(&mgr, &dev);

    TEST_ASSERT_FALSE(A2B_IrqMgrIsEnabled(&mgr));

    A2B_DeviceDeinit(&dev);
}

void test_A2B_IrqMgrEnable(void)
{
    A2B_Device dev;
    A2B_Config config = A2B_CONFIG_DEFAULT;
    A2B_DeviceInit(&dev, &config);

    A2B_IrqMgr mgr;
    A2B_IrqMgrInit(&mgr, &dev);

    A2B_Result res = A2B_IrqMgrEnable(&mgr);

    TEST_ASSERT_EQUAL_INT32(A2B_SUCCESS, res);
    TEST_ASSERT_TRUE(A2B_IrqMgrIsEnabled(&mgr));

    A2B_DeviceDeinit(&dev);
}

void test_A2B_IrqMgrDisable(void)
{
    A2B_Device dev;
    A2B_Config config = A2B_CONFIG_DEFAULT;
    A2B_DeviceInit(&dev, &config);

    A2B_IrqMgr mgr;
    A2B_IrqMgrInit(&mgr, &dev);
    A2B_IrqMgrEnable(&mgr);

    A2B_Result res = A2B_IrqMgrDisable(&mgr);

    TEST_ASSERT_EQUAL_INT32(A2B_SUCCESS, res);
    TEST_ASSERT_FALSE(A2B_IrqMgrIsEnabled(&mgr));

    A2B_DeviceDeinit(&dev);
}

void test_A2B_IrqMgrRegisterCallback(void)
{
    A2B_Device dev;
    A2B_Config config = A2B_CONFIG_DEFAULT;
    A2B_DeviceInit(&dev, &config);

    A2B_IrqMgr mgr;
    A2B_IrqMgrInit(&mgr, &dev);

    A2B_IrqMgrRegisterCallback(&mgr, irq_test_callback, &dev);

    TEST_ASSERT_NOT_NULL(mgr.callback);

    A2B_DeviceDeinit(&dev);
}

void test_A2B_IrqMgrTriggerEvent_calls_callback(void)
{
    A2B_Device dev;
    A2B_Config config = A2B_CONFIG_DEFAULT;
    A2B_DeviceInit(&dev, &config);

    A2B_IrqMgr mgr;
    A2B_IrqMgrInit(&mgr, &dev);

    g_last_irq_event = A2B_IRQ_EVENT_NONE;
    g_last_irq_priv = NULL;

    A2B_IrqMgrRegisterCallback(&mgr, irq_test_callback, &dev);
    A2B_IrqMgrEnable(&mgr);
    A2B_IrqMgrTriggerEvent(&mgr, A2B_IRQ_EVENT_PLL_LOCKED);

    TEST_ASSERT_EQUAL_INT(A2B_IRQ_EVENT_PLL_LOCKED, g_last_irq_event);
    TEST_ASSERT_EQUAL_PTR(&dev, g_last_irq_priv);

    A2B_DeviceDeinit(&dev);
}

void test_A2B_IrqMgrTriggerEvent_no_callback_when_disabled(void)
{
    A2B_Device dev;
    A2B_Config config = A2B_CONFIG_DEFAULT;
    A2B_DeviceInit(&dev, &config);

    A2B_IrqMgr mgr;
    A2B_IrqMgrInit(&mgr, &dev);

    g_last_irq_event = A2B_IRQ_EVENT_NONE;

    A2B_IrqMgrRegisterCallback(&mgr, irq_test_callback, &dev);
    /* Not enabled */
    A2B_IrqMgrTriggerEvent(&mgr, A2B_IRQ_EVENT_PLL_LOCKED);

    TEST_ASSERT_EQUAL_INT(A2B_IRQ_EVENT_NONE, g_last_irq_event);

    A2B_DeviceDeinit(&dev);
}

#include "a2b_device.h"
#include "a2b_logger.h"
#include <stddef.h>

A2B_Result A2B_DeviceInit(A2B_Device *dev, const A2B_Config *config)
{
    if (!dev || !config) {
        return A2B_ERR_INVALID_PARAM;
    }

    /* Initialize bus interface */
    dev->bus = A2B_I2CInit();
    if (!dev->bus) {
        return A2B_ERR_GENERIC;
    }

    /* Copy configuration */
    dev->config.master_address = config->master_address;
    dev->config.bus_speed_hz = config->bus_speed_hz;
    dev->config.max_nodes = (config->max_nodes > A2B_MAX_NODES) ?
                             A2B_MAX_NODES : config->max_nodes;

    /* Initialize state */
    dev->state = A2B_STATE_INIT;
    dev->node_count = 0;
    dev->clock_mode = A2B_CLOCK_MODE_MASTER;
    dev->platform_data = NULL;
    dev->priv = NULL;

    /* Clear node array */
    for (int i = 0; i < A2B_MAX_NODES; i++) {
        dev->nodes[i].id = 0;
        dev->nodes[i].address = 0;
        dev->nodes[i].state = A2B_NODE_STATE_UNKNOWN;
    }

    A2B_LOG_INFO(A2B_LOG_TAG_MAIN, "Device initialized (addr=0x%02X, speed=%lu)",
                 dev->config.master_address, (unsigned long)dev->config.bus_speed_hz);

    return A2B_SUCCESS;
}

A2B_Result A2B_DeviceDeinit(A2B_Device *dev)
{
    if (!dev) {
        return A2B_ERR_INVALID_PARAM;
    }

    dev->state = A2B_STATE_SHUTDOWN;

    if (dev->bus) {
        A2B_I2CDeinit(dev->bus);
        dev->bus = NULL;
    }

    A2B_LOG_INFO(A2B_LOG_TAG_MAIN, "Device deinitialized");

    return A2B_SUCCESS;
}

A2B_State A2B_DeviceGetState(const A2B_Device *dev)
{
    return dev ? dev->state : A2B_STATE_ERROR;
}

A2B_Result A2B_DeviceSetState(A2B_Device *dev, A2B_State state)
{
    if (!dev) {
        return A2B_ERR_INVALID_PARAM;
    }

    A2B_State old_state = dev->state;
    dev->state = state;

    A2B_LOG_INFO(A2B_LOG_TAG_STATE, "State transition: %d -> %d", old_state, state);

    return A2B_SUCCESS;
}

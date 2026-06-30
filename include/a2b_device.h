#ifndef A2B_DEVICE_H
#define A2B_DEVICE_H

#include "a2b_types.h"
#include "a2b_error.h"
#include "a2b_i2c.h"

/**
 * A2B device structure - represents the A2B master
 */
typedef struct _A2B_Device {
    /** Bus interface handle */
    A2B_I2C_HANDLE          bus;

    /** Device configuration */
    A2B_Config              config;

    /** Current state */
    A2B_State               state;

    /** Node array (max 16 nodes) */
    A2B_NodeInfo            nodes[A2B_MAX_NODES];

    /** Number of discovered nodes */
    uint8_t                 node_count;

    /** Master clock mode */
    A2B_ClockMode          clock_mode;

    /** Platform-specific data */
    void                   *platform_data;

    /** Private user data */
    void                   *priv;
} A2B_Device;

/**
 * Initialize A2B device
 */
A2B_Result A2B_DeviceInit(A2B_Device *dev, const A2B_Config *config);

/**
 * Deinitialize A2B device
 */
A2B_Result A2B_DeviceDeinit(A2B_Device *dev);

/**
 * Get device state
 */
A2B_State A2B_DeviceGetState(const A2B_Device *dev);

/**
 * Set device state
 */
A2B_Result A2B_DeviceSetState(A2B_Device *dev, A2B_State state);

#endif /* A2B_DEVICE_H */

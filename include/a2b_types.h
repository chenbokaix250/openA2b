#ifndef A2B_TYPES_H
#define A2B_TYPES_H

#include <stdint.h>
#include <stdbool.h>

/* Handle types - opaque pointers */
typedef struct _A2B_I2C_Ops *A2B_I2C_HANDLE;
typedef struct _A2B_Timer_Ops *A2B_TIMER_HANDLE;
typedef void *A2B_DEVICE_HANDLE;

/* Result code */
typedef int32_t A2B_Result;

/* Boolean convenience */
typedef bool A2B_Bool;

#define A2B_TRUE  true
#define A2B_FALSE false

/* Maximum limits */
#define A2B_MAX_NODES   16
#define A2B_MAX_ROUTES  64

/* Node address type */
typedef uint8_t A2B_NodeAddr;

/* I2C address type */
typedef uint8_t A2B_I2CAddr;

/* Register address type */
typedef uint16_t A2B_RegAddr;

/* State enumeration */
typedef enum {
    A2B_STATE_INIT = 0,
    A2B_STATE_IDLE,
    A2B_STATE_ERROR,
    A2B_STATE_SHUTDOWN
} A2B_State;

/* Node structure (forward declared) */
typedef struct _A2B_Node A2B_Node;

/* Device configuration */
typedef struct _A2B_Config {
    uint8_t  master_address;
    uint32_t bus_speed_hz;
    uint8_t  max_nodes;
} A2B_Config;

/* Default configuration */
#define A2B_CONFIG_DEFAULT { 0x68, 400000, 8 }

#endif /* A2B_TYPES_H */

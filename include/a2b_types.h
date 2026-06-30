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

/* Node state */
typedef enum {
    A2B_NODE_STATE_UNKNOWN = 0,
    A2B_NODE_STATE_ONLINE,
    A2B_NODE_STATE_OFFLINE,
    A2B_NODE_STATE_ERROR
} A2B_NodeState;

/* Clock mode */
typedef enum {
    A2B_CLOCK_MODE_MASTER = 0,
    A2B_CLOCK_MODE_SLAVE
} A2B_ClockMode;

/* Clock status */
typedef struct _A2B_ClockStatus {
    bool pll_locked;
    bool clock_source_valid;
} A2B_ClockStatus;

/* Node info */
typedef struct _A2B_NodeInfo {
    uint8_t                 id;
    A2B_I2CAddr             address;
    uint16_t                vendor_id;
    uint16_t                product_id;
    uint8_t                 revision_id;
    A2B_NodeState           state;
    uint8_t                 discovery_seq;
} A2B_NodeInfo;

/* Route configuration */
typedef struct _A2B_RouteConfig {
    uint8_t     src_node;
    uint8_t     dst_node;
    uint8_t     src_slot;
    uint8_t     dst_slot;
    uint8_t     num_slots;
    bool        enabled;
} A2B_RouteConfig;

/* TDM configuration */
typedef struct _A2B_TDMConfig {
    uint8_t     num_slots;
    uint8_t     slot_size;
    bool        i2s_mode;
} A2B_TDMConfig;

#endif /* A2B_TYPES_H */

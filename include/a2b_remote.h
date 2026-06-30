#ifndef A2B_REMOTE_H
#define A2B_REMOTE_H

#include "a2b_types.h"
#include "a2b_error.h"
#include "a2b_device.h"
#include "ad2428_reg.h"

/**
 * Remote register access context
 */
typedef struct _A2B_RemoteReg {
    A2B_Device         *dev;
    AD2428_RegContext   reg_ctx;
} A2B_RemoteReg;

/**
 * Initialize remote register access
 */
A2B_Result A2B_RemoteRegInit(A2B_RemoteReg *ctx, A2B_Device *dev);

/**
 * Read register from a specific node
 */
A2B_Result A2B_RemoteRegRead(A2B_RemoteReg *ctx, uint8_t node_id,
                                 uint16_t reg, uint32_t *val);

/**
 * Write register to a specific node
 */
A2B_Result A2B_RemoteRegWrite(A2B_RemoteReg *ctx, uint8_t node_id,
                                  uint16_t reg, uint32_t val);

/**
 * Update bits in a register on a specific node
 */
A2B_Result A2B_RemoteRegUpdateBits(A2B_RemoteReg *ctx, uint8_t node_id,
                                       uint16_t reg, uint32_t mask, uint32_t val);

#endif /* A2B_REMOTE_H */

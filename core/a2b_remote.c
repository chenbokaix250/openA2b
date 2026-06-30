#include "a2b_remote.h"
#include "a2b_logger.h"
#include <stddef.h>

static A2B_Result remote_read_fn(void *priv, uint16_t reg, uint32_t *val)
{
    (void)priv;
    (void)reg;
    (void)val;
    /* Stub - in real implementation, would do I2C read */
    return A2B_ERR_UNSUPPORTED;
}

static A2B_Result remote_write_fn(void *priv, uint16_t reg, uint32_t val)
{
    (void)priv;
    (void)reg;
    (void)val;
    /* Stub - in real implementation, would do I2C write */
    return A2B_ERR_UNSUPPORTED;
}

A2B_Result A2B_RemoteRegInit(A2B_RemoteReg *ctx, A2B_Device *dev)
{
    if (!ctx || !dev) {
        return A2B_ERR_INVALID_PARAM;
    }

    ctx->dev = dev;
    AD2428_RegInit(&ctx->reg_ctx, ctx, remote_read_fn, remote_write_fn);

    A2B_LOG_INFO(A2B_LOG_TAG_REG, "Remote register access initialized");

    return A2B_SUCCESS;
}

A2B_Result A2B_RemoteRegRead(A2B_RemoteReg *ctx, uint8_t node_id,
                                 uint16_t reg, uint32_t *val)
{
    (void)ctx;
    (void)node_id;
    (void)reg;
    (void)val;

    /* Stub implementation - returns unsupported */
    A2B_LOG_WARN(A2B_LOG_TAG_REG, "Remote read not implemented (node=%d, reg=0x%02X)",
                  node_id, reg);
    return A2B_ERR_UNSUPPORTED;
}

A2B_Result A2B_RemoteRegWrite(A2B_RemoteReg *ctx, uint8_t node_id,
                                  uint16_t reg, uint32_t val)
{
    (void)ctx;
    (void)node_id;
    (void)reg;
    (void)val;

    /* Stub implementation - returns unsupported */
    A2B_LOG_WARN(A2B_LOG_TAG_REG, "Remote write not implemented (node=%d, reg=0x%02X)",
                  node_id, reg);
    return A2B_ERR_UNSUPPORTED;
}

A2B_Result A2B_RemoteRegUpdateBits(A2B_RemoteReg *ctx, uint8_t node_id,
                                       uint16_t reg, uint32_t mask, uint32_t val)
{
    (void)ctx;
    (void)node_id;
    (void)reg;
    (void)mask;
    (void)val;

    /* Stub implementation - returns unsupported */
    return A2B_ERR_UNSUPPORTED;
}

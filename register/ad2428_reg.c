#include "ad2428_reg.h"

void AD2428_RegInit(AD2428_RegContext *ctx, void *priv,
                     AD2428_RegReadFn read, AD2428_RegWriteFn write)
{
    ctx->priv = priv;
    ctx->read = read;
    ctx->write = write;
}

A2B_Result AD2428_RegRead(AD2428_RegContext *ctx, uint16_t reg, uint32_t *val)
{
    if (ctx->read) {
        return ctx->read(ctx->priv, reg, val);
    }
    return A2B_ERR_UNSUPPORTED;
}

A2B_Result AD2428_RegWrite(AD2428_RegContext *ctx, uint16_t reg, uint32_t val)
{
    if (ctx->write) {
        return ctx->write(ctx->priv, reg, val);
    }
    return A2B_ERR_UNSUPPORTED;
}

A2B_Result AD2428_RegUpdateBits(AD2428_RegContext *ctx, uint16_t reg,
                                  uint32_t mask, uint32_t val)
{
    uint32_t cur = 0;
    A2B_Result res;

    res = AD2428_RegRead(ctx, reg, &cur);
    if (res != A2B_SUCCESS) {
        return res;
    }

    cur = (cur & ~mask) | (val & mask);
    return AD2428_RegWrite(ctx, reg, cur);
}
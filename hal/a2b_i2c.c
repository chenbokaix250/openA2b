#include "a2b_i2c.h"
#include "a2b_build.h"

/* Stub I2C handle data */
typedef struct _A2B_I2C_StubData {
    bool initialized;
} A2B_I2C_StubData;

static A2B_I2C_StubData g_stub_data = { false };

A2B_I2C_HANDLE A2B_I2CInit(void)
{
    g_stub_data.initialized = true;
    return (A2B_I2C_HANDLE)&g_stub_data;
}

A2B_Result A2B_I2CDeinit(A2B_I2C_HANDLE handle)
{
    (void)handle;
    g_stub_data.initialized = false;
    return A2B_SUCCESS;
}

A2B_Result A2B_I2CWrite(A2B_I2C_HANDLE handle, A2B_I2CAddr addr,
                         const uint8_t *data, uint32_t len)
{
    (void)handle; (void)addr; (void)data; (void)len;
    return A2B_ERR_UNSUPPORTED;  /* Stub: not implemented */
}

A2B_Result A2B_I2CRead(A2B_I2C_HANDLE handle, A2B_I2CAddr addr,
                        uint8_t *data, uint32_t len)
{
    (void)handle; (void)addr; (void)data; (void)len;
    return A2B_ERR_UNSUPPORTED;  /* Stub: not implemented */
}

A2B_Result A2B_I2CWriteRead(A2B_I2C_HANDLE handle, A2B_I2CAddr addr,
                              const uint8_t *wdata, uint32_t wlen,
                              uint8_t *rdata, uint32_t rlen)
{
    (void)handle; (void)addr; (void)wdata; (void)wlen; (void)rdata; (void)rlen;
    return A2B_ERR_UNSUPPORTED;  /* Stub: not implemented */
}
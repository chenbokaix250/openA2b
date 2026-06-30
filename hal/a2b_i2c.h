#ifndef A2B_I2C_H
#define A2B_I2C_H

#include "a2b_types.h"
#include "a2b_error.h"

/**
 * I2C operations interface
 * All I2C access goes through this abstraction.
 */
typedef struct _A2B_I2C_Ops {
    A2B_Result (*init)(A2B_I2C_HANDLE handle, void *config);
    A2B_Result (*deinit)(A2B_I2C_HANDLE handle);
    A2B_Result (*write)(A2B_I2C_HANDLE handle, A2B_I2CAddr addr,
                         const uint8_t *data, uint32_t len);
    A2B_Result (*read)(A2B_I2C_HANDLE handle, A2B_I2CAddr addr,
                        uint8_t *data, uint32_t len);
    A2B_Result (*write_read)(A2B_I2C_HANDLE handle, A2B_I2CAddr addr,
                              const uint8_t *wdata, uint32_t wlen,
                              uint8_t *rdata, uint32_t rlen);
} A2B_I2C_Ops;

/**
 * I2C handle - opaque pointer to platform-specific instance
 */
struct _A2B_I2C_Handle {
    const A2B_I2C_Ops *ops;
    void *priv;
};

/**
 * Initialize default I2C HAL
 */
A2B_I2C_HANDLE A2B_I2CInit(void);

/**
 * Close I2C HAL
 */
A2B_Result A2B_I2CDeinit(A2B_I2C_HANDLE handle);

/**
 * Write data to I2C device
 */
A2B_Result A2B_I2CWrite(A2B_I2C_HANDLE handle, A2B_I2CAddr addr,
                         const uint8_t *data, uint32_t len);

/**
 * Read data from I2C device
 */
A2B_Result A2B_I2CRead(A2B_I2C_HANDLE handle, A2B_I2CAddr addr,
                        uint8_t *data, uint32_t len);

/**
 * Combined write-then-read (repeated start)
 */
A2B_Result A2B_I2CWriteRead(A2B_I2C_HANDLE handle, A2B_I2CAddr addr,
                              const uint8_t *wdata, uint32_t wlen,
                              uint8_t *rdata, uint32_t rlen);

#endif /* A2B_I2C_H */
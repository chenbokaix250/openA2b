#ifndef MOCK_I2C_H
#define MOCK_I2C_H

#include "a2b_types.h"
#include "a2b_error.h"
#include "a2b_i2c.h"

/**
 * Mock I2C device response record
 */
typedef struct _MockI2C_Response {
    A2B_I2CAddr addr;
    uint8_t     reg_addr;
    uint32_t    read_count;
    uint32_t    write_count;
    bool        nack_on_write;
    bool        nack_on_read;
} MockI2C_Response;

/**
 * Initialize mock I2C
 */
void MockI2C_Init(void);

/**
 * Reset mock I2C state
 */
void MockI2C_Reset(void);

/**
 * Configure a slave to NACK on next write
 */
void MockI2C_InjectNackOnWrite(A2B_I2CAddr addr);

/**
 * Configure a slave to NACK on next read
 */
void MockI2C_InjectNackOnRead(A2B_I2CAddr addr);

/**
 * Get response record for an address
 */
const MockI2C_Response *MockI2C_GetResponse(A2B_I2CAddr addr);

#endif /* MOCK_I2C_H */
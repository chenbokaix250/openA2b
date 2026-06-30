#ifndef MOCK_AD2428_H
#define MOCK_AD2428_H

#include "a2b_types.h"
#include "a2b_error.h"
#include "a2b_i2c.h"

/* Mock AD2428 register bank */
typedef struct _MockAD2428_RegBank {
    uint8_t  chip_id;
    uint8_t  rev_id;
    uint8_t  status;
    uint8_t  control;
    uint8_t  i2c_addr;
    uint8_t  disc_stat;
    uint8_t  disc_cfg;
    uint8_t  node_cfg;
    uint8_t  pll_ctl;
    uint8_t  pll_sr;
    uint8_t  int_src;
    uint8_t  int_mask;
} MockAD2428_RegBank;

/* Mock AD2428 device */
typedef struct _MockAD2428 {
    A2B_I2CAddr         i2c_addr;
    MockAD2428_RegBank  regs;
    bool                nack_next_write;
} MockAD2428;

/**
 * Initialize mock AD2428
 */
void MockAD2428_Init(MockAD2428 *dev, A2B_I2CAddr addr);

/**
 * Reset mock AD2428 to default state
 */
void MockAD2428_Reset(MockAD2428 *dev);

/**
 * Set register value
 */
void MockAD2428_SetReg(MockAD2428 *dev, uint8_t reg, uint8_t val);

/**
 * Get register value
 */
uint8_t MockAD2428_GetReg(const MockAD2428 *dev, uint8_t reg);

/**
 * Inject NACK on next write
 */
void MockAD2428_InjectNack(MockAD2428 *dev);

/**
 * Simulate I2C write operation
 */
A2B_Result MockAD2428_I2CWrite(MockAD2428 *dev, uint8_t reg,
                                 const uint8_t *data, uint32_t len);

/**
 * Simulate I2C read operation
 */
A2B_Result MockAD2428_I2CRead(MockAD2428 *dev, uint8_t reg,
                                uint8_t *data, uint32_t len);

#endif /* MOCK_AD2428_H */

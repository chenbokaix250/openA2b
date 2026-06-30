#ifndef AD2428_REG_H
#define AD2428_REG_H

#include "a2b_types.h"
#include "a2b_error.h"
#include "a2b_i2c.h"

/* AD2428 Register Base Address */
#define AD2428_REG_BASE          0x00

/* Common Registers */
#define AD2428_REG_CHIP_ID       0x00  /* Chip ID (read-only) */
#define AD2428_REG_REV_ID        0x01  /* Revision ID (read-only) */
#define AD2428_REG_STATUS        0x02  /* Status register */
#define AD2428_REG_CONTROL       0x03  /* Control register */

/* I2C Configuration Registers */
#define AD2428_REG_I2C_ADDR      0x04  /* I2C slave address */
#define AD2428_REG_I2CGCFG       0x05  /* I2C general config */
#define AD2428_REG_I2CCMD        0x06  /* I2C command */

/* Discovery Registers */
#define AD2428_REG_DISCSTAT      0x0A  /* Discovery status */
#define AD2428_REG_DISCCFG       0x0B  /* Discovery config */
#define AD2428_REG_NODECFG       0x0C  /* Node config */
#define AD2428_REG_NODEBC        0x0D  /* Node burst count */

/* Clock Registers */
#define AD2428_REG_PLLCTL        0x10  /* PLL control */
#define AD2428_REG_PLLSR         0x11  /* PLL status */
#define AD2428_REG_CLKSEL        0x12  /* Clock select */

/* Interrupt Registers */
#define AD2428_REG_INTSRC        0x14  /* Interrupt source */
#define AD2428_REG_INTMASK       0x15  /* Interrupt mask */
#define AD2428_REG_INTPOL        0x16  /* Interrupt polarity */

/* Audio Registers */
#define AD2428_REG_TDMSLOT0      0x20  /* TDM slot 0 */
#define AD2428_REG_TDMSLOT1      0x21  /* TDM slot 1 */
#define AD2428_REG_TDMSLOT2      0x22  /* TDM slot 2 */
#define AD2428_REG_TDMSLOT3      0x23  /* TDM slot 3 */
#define AD2428_REG_TDMCFG        0x24  /* TDM config */

/* Node registers (0x30-0x3F) - per-node configuration */
#define AD2428_REG_NODE_BASE      0x30
#define AD2428_REG_NODEn(n)       (AD2428_REG_NODE_BASE + (n))

/* Mask and shift helpers for bitfields */
#define AD2428_MASK(bit)          (1U << (bit))
#define AD2428_SHIFT(val, bit)    (((val) & AD2428_MASK(bit)) >> (bit))

/* Common bit definitions */
#define AD2428_STATUS_PLL_LOCK    AD2428_MASK(0)
#define AD2428_STATUS_DISC_DONE  AD2428_MASK(1)
#define AD2428_STATUS_ERR        AD2428_MASK(2)

/* Control register bits */
#define AD2428_CONTROL_RESET      AD2428_MASK(0)
#define AD2428_CONTROL_PWRDN      AD2428_MASK(1)
#define AD2428_CONTROL_START      AD2428_MASK(2)

/* Result type for register access */
typedef A2B_Result (*AD2428_RegReadFn)(void *priv, uint16_t reg, uint32_t *val);
typedef A2B_Result (*AD2428_RegWriteFn)(void *priv, uint16_t reg, uint32_t val);

/**
 * AD2428 register access context
 */
typedef struct _AD2428_RegContext {
    void *priv;
    AD2428_RegReadFn read;
    AD2428_RegWriteFn write;
} AD2428_RegContext;

/**
 * Initialize register context
 */
void AD2428_RegInit(AD2428_RegContext *ctx, void *priv,
                     AD2428_RegReadFn read, AD2428_RegWriteFn write);

/**
 * Read register
 */
A2B_Result AD2428_RegRead(AD2428_RegContext *ctx, uint16_t reg, uint32_t *val);

/**
 * Write register
 */
A2B_Result AD2428_RegWrite(AD2428_RegContext *ctx, uint16_t reg, uint32_t val);

/**
 * Update specific bits in a register
 */
A2B_Result AD2428_RegUpdateBits(AD2428_RegContext *ctx, uint16_t reg,
                                  uint32_t mask, uint32_t val);

#endif /* AD2428_REG_H */
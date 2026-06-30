#include "mock_ad2428.h"

void MockAD2428_Init(MockAD2428 *dev, A2B_I2CAddr addr)
{
    if (!dev) return;

    dev->i2c_addr = addr;
    MockAD2428_Reset(dev);
}

void MockAD2428_Reset(MockAD2428 *dev)
{
    if (!dev) return;

    dev->regs.chip_id = 0x28;   /* AD2428 chip ID */
    dev->regs.rev_id = 0x10;    /* Revision A */
    dev->regs.status = 0x00;
    dev->regs.control = 0x00;
    dev->regs.i2c_addr = dev->i2c_addr;
    dev->regs.disc_stat = 0x00;
    dev->regs.disc_cfg = 0x00;
    dev->regs.node_cfg = 0x00;
    dev->regs.pll_ctl = 0x00;
    dev->regs.pll_sr = 0x00;
    dev->regs.int_src = 0x00;
    dev->regs.int_mask = 0xFF;
    dev->nack_next_write = false;
}

void MockAD2428_SetReg(MockAD2428 *dev, uint8_t reg, uint8_t val)
{
    if (!dev || reg > 0x0F) return;

    switch (reg) {
        case 0x00: dev->regs.chip_id = val; break;
        case 0x01: dev->regs.rev_id = val; break;
        case 0x02: dev->regs.status = val; break;
        case 0x03: dev->regs.control = val; break;
        case 0x04: dev->regs.i2c_addr = val; break;
        case 0x0A: dev->regs.disc_stat = val; break;
        case 0x0B: dev->regs.disc_cfg = val; break;
        case 0x0C: dev->regs.node_cfg = val; break;
        case 0x10: dev->regs.pll_ctl = val; break;
        case 0x11: dev->regs.pll_sr = val; break;
        case 0x14: dev->regs.int_src = val; break;
        case 0x15: dev->regs.int_mask = val; break;
    }
}

uint8_t MockAD2428_GetReg(const MockAD2428 *dev, uint8_t reg)
{
    if (!dev || reg > 0x0F) return 0;

    switch (reg) {
        case 0x00: return dev->regs.chip_id;
        case 0x01: return dev->regs.rev_id;
        case 0x02: return dev->regs.status;
        case 0x03: return dev->regs.control;
        case 0x04: return dev->regs.i2c_addr;
        case 0x0A: return dev->regs.disc_stat;
        case 0x0B: return dev->regs.disc_cfg;
        case 0x0C: return dev->regs.node_cfg;
        case 0x10: return dev->regs.pll_ctl;
        case 0x11: return dev->regs.pll_sr;
        case 0x14: return dev->regs.int_src;
        case 0x15: return dev->regs.int_mask;
    }
    return 0;
}

void MockAD2428_InjectNack(MockAD2428 *dev)
{
    if (dev) {
        dev->nack_next_write = true;
    }
}

A2B_Result MockAD2428_I2CWrite(MockAD2428 *dev, uint8_t reg,
                                 const uint8_t *data, uint32_t len)
{
    if (!dev) {
        return A2B_ERR_INVALID_PARAM;
    }

    if (dev->nack_next_write) {
        dev->nack_next_write = false;
        return A2B_ERR_I2C_NACK;
    }

    if (len > 0) {
        MockAD2428_SetReg(dev, reg, data[0]);
    }

    return A2B_SUCCESS;
}

A2B_Result MockAD2428_I2CRead(MockAD2428 *dev, uint8_t reg,
                                uint8_t *data, uint32_t len)
{
    if (!dev || !data || len == 0) {
        return A2B_ERR_INVALID_PARAM;
    }

    data[0] = MockAD2428_GetReg(dev, reg);

    return A2B_SUCCESS;
}

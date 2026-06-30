#include "mock_i2c.h"

static MockI2C_Response g_responses[256] = { 0 };

void MockI2C_Init(void)
{
    MockI2C_Reset();
}

void MockI2C_Reset(void)
{
    for (int i = 0; i < 256; i++) {
        g_responses[i].addr = (A2B_I2CAddr)i;
        g_responses[i].reg_addr = 0;
        g_responses[i].read_count = 0;
        g_responses[i].write_count = 0;
        g_responses[i].nack_on_write = false;
        g_responses[i].nack_on_read = false;
    }
}

void MockI2C_InjectNackOnWrite(A2B_I2CAddr addr)
{
    g_responses[addr].nack_on_write = true;
}

void MockI2C_InjectNackOnRead(A2B_I2CAddr addr)
{
    g_responses[addr].nack_on_read = true;
}

const MockI2C_Response *MockI2C_GetResponse(A2B_I2CAddr addr)
{
    return &g_responses[addr];
}
#include "unity.h"
#include "ad2428_reg.h"

/* Test context that captures calls */
static uint16_t g_last_read_reg = 0;
static uint32_t g_read_value = 0x12345678;
static uint16_t g_last_write_reg = 0;
static uint32_t g_write_value = 0;
static int g_read_calls = 0;
static int g_write_calls = 0;

static A2B_Result test_read(void *priv, uint16_t reg, uint32_t *val)
{
    (void)priv;
    g_last_read_reg = reg;
    *val = g_read_value;
    g_read_calls++;
    return A2B_SUCCESS;
}

static A2B_Result test_write(void *priv, uint16_t reg, uint32_t val)
{
    (void)priv;
    g_last_write_reg = reg;
    g_write_value = val;
    g_write_calls++;
    return A2B_SUCCESS;
}

/* Reset function called by setUp in test_main.c */
void AD2428_RegTestReset(void)
{
    g_last_read_reg = 0;
    g_read_value = 0x12345678;
    g_last_write_reg = 0;
    g_write_value = 0;
    g_read_calls = 0;
    g_write_calls = 0;
}

void test_AD2428_RegInit_sets_context(void)
{
    AD2428_RegContext ctx;
    AD2428_RegInit(&ctx, NULL, test_read, test_write);
    TEST_ASSERT_EQUAL_PTR(test_read, ctx.read);
    TEST_ASSERT_EQUAL_PTR(test_write, ctx.write);
}

void test_AD2428_RegRead_calls_read_fn(void)
{
    AD2428_RegContext ctx;
    AD2428_RegInit(&ctx, NULL, test_read, test_write);

    uint32_t val = 0;
    A2B_Result res = AD2428_RegRead(&ctx, 0x0A, &val);

    TEST_ASSERT_EQUAL_INT32(A2B_SUCCESS, res);
    TEST_ASSERT_EQUAL_UINT16(0x0A, g_last_read_reg);
    TEST_ASSERT_EQUAL_UINT32(0x12345678, val);
}

void test_AD2428_RegWrite_calls_write_fn(void)
{
    AD2428_RegContext ctx;
    AD2428_RegInit(&ctx, NULL, test_read, test_write);

    A2B_Result res = AD2428_RegWrite(&ctx, 0x03, 0xDEADBEEF);

    TEST_ASSERT_EQUAL_INT32(A2B_SUCCESS, res);
    TEST_ASSERT_EQUAL_UINT16(0x03, g_last_write_reg);
    TEST_ASSERT_EQUAL_UINT32(0xDEADBEEF, g_write_value);
}

void test_AD2428_RegUpdateBits_reads_modifies_writes(void)
{
    AD2428_RegContext ctx;
    AD2428_RegInit(&ctx, NULL, test_read, test_write);

    /* Simulate current register value */
    g_read_value = 0x0000FFFF;

    A2B_Result res = AD2428_RegUpdateBits(&ctx, 0x03, 0x0000FF00, 0x0000AA00);

    TEST_ASSERT_EQUAL_INT32(A2B_SUCCESS, res);
    TEST_ASSERT_EQUAL_INT(1, g_read_calls);   /* Read first */
    TEST_ASSERT_EQUAL_INT(1, g_write_calls);  /* Then write */
    /* Should have masked and shifted correctly */
    TEST_ASSERT_EQUAL_UINT32(0x0000AA00, g_write_value & 0x0000FF00);
}
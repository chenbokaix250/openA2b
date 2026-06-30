#include "a2b_clock.h"
#include "a2b_logger.h"

void A2B_ClockMgrInit(A2B_ClockMgr *mgr, A2B_Device *dev)
{
    if (!mgr || !dev) return;

    mgr->dev = dev;
    mgr->pll_status = A2B_PLL_UNLOCKED;
    mgr->config.source = A2B_CLOCK_SRC_LOCAL;
    mgr->config.target_freq_hz = 0;
    mgr->config.pll_multiplier = 1;

    A2B_LOG_INFO(A2B_LOG_TAG_CLOCK, "Clock manager initialized");
}

A2B_Result A2B_ClockMgrConfigure(A2B_ClockMgr *mgr, const A2B_ClockConfig *config)
{
    if (!mgr || !config) {
        return A2B_ERR_INVALID_PARAM;
    }

    mgr->config = *config;

    A2B_LOG_INFO(A2B_LOG_TAG_CLOCK, "Clock configured: source=%d, freq=%lu Hz",
                 config->source, (unsigned long)config->target_freq_hz);

    return A2B_SUCCESS;
}

A2B_Result A2B_ClockMgrStartPLL(A2B_ClockMgr *mgr)
{
    if (!mgr) {
        return A2B_ERR_INVALID_PARAM;
    }

    mgr->pll_status = A2B_PLL_LOCKING;

    A2B_LOG_INFO(A2B_LOG_TAG_CLOCK, "PLL locking started");

    return A2B_SUCCESS;
}

A2B_Result A2B_ClockMgrWaitLock(A2B_ClockMgr *mgr, uint32_t timeout_ms)
{
    if (!mgr) {
        return A2B_ERR_INVALID_PARAM;
    }

    (void)timeout_ms;  /* Stub - would block in real implementation */

    if (mgr->pll_status == A2B_PLL_LOCKING) {
        mgr->pll_status = A2B_PLL_LOCKED;
    }

    if (mgr->pll_status == A2B_PLL_LOCKED) {
        A2B_LOG_INFO(A2B_LOG_TAG_CLOCK, "PLL locked");
        return A2B_SUCCESS;
    }

    A2B_LOG_ERROR(A2B_LOG_TAG_CLOCK, "PLL lock failed");
    return A2B_ERR_TIMEOUT;
}

A2B_PLLStatus A2B_ClockMgrGetPLLStatus(const A2B_ClockMgr *mgr)
{
    return mgr ? mgr->pll_status : A2B_PLL_UNLOCKED;
}

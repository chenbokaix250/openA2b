#ifndef A2B_CLOCK_H
#define A2B_CLOCK_H

#include "a2b_types.h"
#include "a2b_error.h"
#include "a2b_device.h"

/* Clock source */
typedef enum {
    A2B_CLOCK_SRC_LOCAL = 0,    /* Master provides clock */
    A2B_CLOCK_SRC_EXTERNAL,      /* External clock input */
    A2B_CLOCK_SRC_RECOVERY       /* Clock recovered from data */
} A2B_ClockSource;

/* PLL status */
typedef enum {
    A2B_PLL_UNLOCKED = 0,
    A2B_PLL_LOCKING,
    A2B_PLL_LOCKED
} A2B_PLLStatus;

/* Clock configuration */
typedef struct _A2B_ClockConfig {
    A2B_ClockSource  source;
    uint32_t         target_freq_hz;
    uint8_t          pll_multiplier;
} A2B_ClockConfig;

/* Clock manager context */
typedef struct _A2B_ClockMgr {
    A2B_Device       *dev;
    A2B_ClockConfig  config;
    A2B_PLLStatus     pll_status;
} A2B_ClockMgr;

/**
 * Initialize clock manager
 */
void A2B_ClockMgrInit(A2B_ClockMgr *mgr, A2B_Device *dev);

/**
 * Configure clock
 */
A2B_Result A2B_ClockMgrConfigure(A2B_ClockMgr *mgr, const A2B_ClockConfig *config);

/**
 * Start PLL locking sequence
 */
A2B_Result A2B_ClockMgrStartPLL(A2B_ClockMgr *mgr);

/**
 * Wait for PLL lock
 */
A2B_Result A2B_ClockMgrWaitLock(A2B_ClockMgr *mgr, uint32_t timeout_ms);

/**
 * Get PLL status
 */
A2B_PLLStatus A2B_ClockMgrGetPLLStatus(const A2B_ClockMgr *mgr);

#endif /* A2B_CLOCK_H */

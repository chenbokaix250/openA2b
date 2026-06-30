#ifndef A2B_SLOT_H
#define A2B_SLOT_H

#include "a2b_types.h"
#include "a2b_error.h"
#include "a2b_device.h"

/* TDM slot configuration */
typedef struct _A2B_SlotConfig {
    uint8_t     slot_index;
    uint8_t     slot_width;      /* bits per slot (8, 16, 24, 32) */
    bool        left_justified;
    bool        inverted_clock;
    uint8_t     delay_bits;       /* delay after frame sync */
} A2B_SlotConfig;

/* Slot manager context */
typedef struct _A2B_SlotMgr {
    A2B_Device     *dev;
    A2B_SlotConfig *slots;
    uint8_t         max_slots;
    uint8_t         slot_count;
} A2B_SlotMgr;

/**
 * Initialize slot manager
 */
void A2B_SlotMgrInit(A2B_SlotMgr *mgr, A2B_Device *dev, uint8_t max_slots);

/**
 * Configure a slot
 */
A2B_Result A2B_SlotMgrSetSlot(A2B_SlotMgr *mgr, const A2B_SlotConfig *slot);

/**
 * Get slot configuration
 */
A2B_Result A2B_SlotMgrGetSlot(const A2B_SlotMgr *mgr, uint8_t index,
                                 A2B_SlotConfig *slot);

/**
 * Get slot count
 */
uint8_t A2B_SlotMgrGetSlotCount(const A2B_SlotMgr *mgr);

#endif /* A2B_SLOT_H */

#include "a2b_slot.h"
#include "a2b_logger.h"
#include <stddef.h>

void A2B_SlotMgrInit(A2B_SlotMgr *mgr, A2B_Device *dev, uint8_t max_slots)
{
    if (!mgr || !dev) return;

    mgr->dev = dev;
    mgr->max_slots = max_slots;
    mgr->slot_count = 0;
    mgr->slots = NULL;

    A2B_LOG_INFO(A2B_LOG_TAG_ROUTE, "Slot manager initialized (max_slots=%d)",
                 max_slots);
}

A2B_Result A2B_SlotMgrSetSlot(A2B_SlotMgr *mgr, const A2B_SlotConfig *slot)
{
    if (!mgr || !slot) {
        return A2B_ERR_INVALID_PARAM;
    }

    if (slot->slot_index >= mgr->max_slots) {
        A2B_LOG_ERROR(A2B_LOG_TAG_ROUTE, "Slot index out of range (max=%d)",
                     mgr->max_slots);
        return A2B_ERR_INVALID_PARAM;
    }

    mgr->slots[slot->slot_index] = *slot;

    if (slot->slot_index >= mgr->slot_count) {
        mgr->slot_count = slot->slot_index + 1;
    }

    A2B_LOG_INFO(A2B_LOG_TAG_ROUTE, "Slot %d configured (width=%d bits)",
                 slot->slot_index, slot->slot_width);

    return A2B_SUCCESS;
}

A2B_Result A2B_SlotMgrGetSlot(const A2B_SlotMgr *mgr, uint8_t index,
                                 A2B_SlotConfig *slot)
{
    if (!mgr || !slot) {
        return A2B_ERR_INVALID_PARAM;
    }

    if (index >= mgr->slot_count) {
        return A2B_ERR_INVALID_PARAM;
    }

    *slot = mgr->slots[index];

    return A2B_SUCCESS;
}

uint8_t A2B_SlotMgrGetSlotCount(const A2B_SlotMgr *mgr)
{
    return mgr ? mgr->slot_count : 0;
}

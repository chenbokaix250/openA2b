#ifndef A2B_IRQ_H
#define A2B_IRQ_H

#include "a2b_types.h"
#include "a2b_error.h"
#include "a2b_device.h"

/* IRQ event types */
typedef enum {
    A2B_IRQ_EVENT_NONE = 0,
    A2B_IRQ_EVENT_PLL_LOCKED,
    A2B_IRQ_EVENT_DISC_COMPLETE,
    A2B_IRQ_EVENT_NODE_DOWN,
    A2B_IRQ_EVENT_CRC_ERROR,
    A2B_IRQ_EVENT_I2C_ERROR
} A2B_IrqEvent;

/* IRQ callback */
typedef void (*A2B_IrqCallback)(A2B_IrqEvent event, void *priv);

/**
 * IRQ manager context
 */
typedef struct _A2B_IrqMgr {
    A2B_Device       *dev;
    A2B_IrqCallback  callback;
    void            *callback_priv;
    bool             enabled;
} A2B_IrqMgr;

/**
 * Initialize IRQ manager
 */
void A2B_IrqMgrInit(A2B_IrqMgr *mgr, A2B_Device *dev);

/**
 * Register IRQ callback
 */
void A2B_IrqMgrRegisterCallback(A2B_IrqMgr *mgr, A2B_IrqCallback callback,
                                 void *priv);

/**
 * Enable IRQ handling
 */
A2B_Result A2B_IrqMgrEnable(A2B_IrqMgr *mgr);

/**
 * Disable IRQ handling
 */
A2B_Result A2B_IrqMgrDisable(A2B_IrqMgr *mgr);

/**
 * Trigger IRQ event (for testing)
 */
void A2B_IrqMgrTriggerEvent(A2B_IrqMgr *mgr, A2B_IrqEvent event);

/**
 * Check if IRQ is enabled
 */
bool A2B_IrqMgrIsEnabled(const A2B_IrqMgr *mgr);

#endif /* A2B_IRQ_H */

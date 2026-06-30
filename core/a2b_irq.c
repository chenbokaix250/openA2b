#include "a2b_irq.h"
#include "a2b_logger.h"
#include <stddef.h>

void A2B_IrqMgrInit(A2B_IrqMgr *mgr, A2B_Device *dev)
{
    if (!mgr || !dev) return;

    mgr->dev = dev;
    mgr->callback = NULL;
    mgr->callback_priv = NULL;
    mgr->enabled = false;

    A2B_LOG_INFO(A2B_LOG_TAG_IRQ, "IRQ manager initialized");
}

void A2B_IrqMgrRegisterCallback(A2B_IrqMgr *mgr, A2B_IrqCallback callback,
                                 void *priv)
{
    if (!mgr) return;

    mgr->callback = callback;
    mgr->callback_priv = priv;

    A2B_LOG_INFO(A2B_LOG_TAG_IRQ, "IRQ callback registered");
}

A2B_Result A2B_IrqMgrEnable(A2B_IrqMgr *mgr)
{
    if (!mgr) {
        return A2B_ERR_INVALID_PARAM;
    }

    mgr->enabled = true;
    A2B_LOG_INFO(A2B_LOG_TAG_IRQ, "IRQ enabled");

    return A2B_SUCCESS;
}

A2B_Result A2B_IrqMgrDisable(A2B_IrqMgr *mgr)
{
    if (!mgr) {
        return A2B_ERR_INVALID_PARAM;
    }

    mgr->enabled = false;
    A2B_LOG_INFO(A2B_LOG_TAG_IRQ, "IRQ disabled");

    return A2B_SUCCESS;
}

void A2B_IrqMgrTriggerEvent(A2B_IrqMgr *mgr, A2B_IrqEvent event)
{
    if (!mgr || !mgr->enabled) {
        return;
    }

    A2B_LOG_INFO(A2B_LOG_TAG_IRQ, "IRQ event: %d", event);

    if (mgr->callback) {
        mgr->callback(event, mgr->callback_priv);
    }
}

bool A2B_IrqMgrIsEnabled(const A2B_IrqMgr *mgr)
{
    return mgr ? mgr->enabled : false;
}

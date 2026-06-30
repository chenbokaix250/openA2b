#include "a2b_recovery.h"
#include "a2b_logger.h"

void A2B_RecoveryMgrInit(A2B_RecoveryMgr *mgr, A2B_Device *dev,
                            A2B_NetworkMgr *net_mgr, A2B_DiagMgr *diag_mgr)
{
    if (!mgr || !dev || !net_mgr || !diag_mgr) return;

    mgr->dev = dev;
    mgr->net_mgr = net_mgr;
    mgr->diag_mgr = diag_mgr;
    mgr->status = A2B_RECOVERY_IDLE;
    mgr->strategy = A2B_RECOVERY_NONE;
    mgr->failed_node = 0;

    A2B_LOG_INFO(A2B_LOG_TAG_MAIN, "Recovery manager initialized");
}

A2B_Result A2B_RecoveryMgrRecover(A2B_RecoveryMgr *mgr,
                                     A2B_RecoveryStrategy strategy,
                                     uint8_t node_id)
{
    if (!mgr) {
        return A2B_ERR_INVALID_PARAM;
    }

    mgr->status = A2B_RECOVERY_IN_PROGRESS;
    mgr->strategy = strategy;
    mgr->failed_node = node_id;

    A2B_LOG_WARN(A2B_LOG_TAG_MAIN, "Recovery started: strategy=%d, node=%d",
                 strategy, node_id);

    /* Simulate recovery */
    switch (strategy) {
        case A2B_RECOVERY_NODE_RESET:
        case A2B_RECOVERY_LINK_RENEGOTIATE:
        case A2B_RECOVERY_FULL_DISCOVERY:
            mgr->status = A2B_RECOVERY_SUCCESS;
            break;
        default:
            mgr->status = A2B_RECOVERY_FAILED;
            break;
    }

    A2B_LOG_INFO(A2B_LOG_TAG_MAIN, "Recovery completed: status=%d", mgr->status);

    return (mgr->status == A2B_RECOVERY_SUCCESS) ? A2B_SUCCESS : A2B_ERR_GENERIC;
}

A2B_RecoveryStatus A2B_RecoveryMgrGetStatus(const A2B_RecoveryMgr *mgr)
{
    return mgr ? mgr->status : A2B_RECOVERY_FAILED;
}

A2B_Result A2B_RecoveryMgrAbort(A2B_RecoveryMgr *mgr)
{
    if (!mgr) {
        return A2B_ERR_INVALID_PARAM;
    }

    mgr->status = A2B_RECOVERY_IDLE;
    A2B_LOG_WARN(A2B_LOG_TAG_MAIN, "Recovery aborted");

    return A2B_SUCCESS;
}

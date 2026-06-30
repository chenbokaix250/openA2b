#ifndef A2B_RECOVERY_H
#define A2B_RECOVERY_H

#include "a2b_types.h"
#include "a2b_error.h"
#include "a2b_device.h"
#include "a2b_network.h"
#include "a2b_diagnostic.h"

/* Recovery strategy */
typedef enum {
    A2B_RECOVERY_NONE = 0,
    A2B_RECOVERY_NODE_RESET,
    A2B_RECOVERY_LINK_RENEGOTIATE,
    A2B_RECOVERY_FULL_DISCOVERY
} A2B_RecoveryStrategy;

/* Recovery status */
typedef enum {
    A2B_RECOVERY_IDLE = 0,
    A2B_RECOVERY_IN_PROGRESS,
    A2B_RECOVERY_SUCCESS,
    A2B_RECOVERY_FAILED
} A2B_RecoveryStatus;

/* Recovery manager context */
typedef struct _A2B_RecoveryMgr {
    A2B_Device            *dev;
    A2B_NetworkMgr        *net_mgr;
    A2B_DiagMgr           *diag_mgr;
    A2B_RecoveryStatus    status;
    A2B_RecoveryStrategy  strategy;
    uint8_t               failed_node;
} A2B_RecoveryMgr;

/**
 * Initialize recovery manager
 */
void A2B_RecoveryMgrInit(A2B_RecoveryMgr *mgr, A2B_Device *dev,
                            A2B_NetworkMgr *net_mgr, A2B_DiagMgr *diag_mgr);

/**
 * Attempt recovery with given strategy
 */
A2B_Result A2B_RecoveryMgrRecover(A2B_RecoveryMgr *mgr,
                                     A2B_RecoveryStrategy strategy,
                                     uint8_t node_id);

/**
 * Get recovery status
 */
A2B_RecoveryStatus A2B_RecoveryMgrGetStatus(const A2B_RecoveryMgr *mgr);

/**
 * Abort recovery
 */
A2B_Result A2B_RecoveryMgrAbort(A2B_RecoveryMgr *mgr);

#endif /* A2B_RECOVERY_H */

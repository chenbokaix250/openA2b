#ifndef A2B_DISCOVERY_H
#define A2B_DISCOVERY_H

#include "a2b_types.h"
#include "a2b_error.h"
#include "a2b_device.h"
#include "a2b_network.h"

/* Discovery status */
typedef enum {
    A2B_DISC_STATUS_NOT_STARTED = 0,
    A2B_DISC_STATUS_IN_PROGRESS,
    A2B_DISC_STATUS_SUCCESS,
    A2B_DISC_STATUS_FAILED
} A2B_DiscoveryStatus;

/* Discovery result */
typedef struct _A2B_DiscoveryResult {
    A2B_DiscoveryStatus  status;
    uint8_t             node_count;
    A2B_Result          error_code;
} A2B_DiscoveryResult;

/**
 * Discovery manager context
 */
typedef struct _A2B_DiscoveryMgr {
    A2B_Device              *dev;
    A2B_NetworkMgr          *net_mgr;
    A2B_DiscoveryStatus     status;
    uint8_t                 current_node_id;
} A2B_DiscoveryMgr;

/**
 * Initialize discovery manager
 */
void A2B_DiscoveryMgrInit(A2B_DiscoveryMgr *mgr, A2B_Device *dev,
                            A2B_NetworkMgr *net_mgr);

/**
 * Start discovery process
 */
A2B_Result A2B_DiscoveryStart(A2B_DiscoveryMgr *mgr);

/**
 * Discover next node (single step)
 */
A2B_Result A2B_DiscoveryStep(A2B_DiscoveryMgr *mgr);

/**
 * Complete discovery
 */
A2B_Result A2B_DiscoveryComplete(A2B_DiscoveryMgr *mgr);

/**
 * Abort discovery
 */
A2B_Result A2B_DiscoveryAbort(A2B_DiscoveryMgr *mgr);

/**
 * Get discovery status
 */
A2B_DiscoveryStatus A2B_DiscoveryGetStatus(const A2B_DiscoveryMgr *mgr);

#endif /* A2B_DISCOVERY_H */

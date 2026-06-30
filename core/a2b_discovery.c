#include "a2b_discovery.h"
#include "a2b_logger.h"
#include "a2b_state.h"

void A2B_DiscoveryMgrInit(A2B_DiscoveryMgr *mgr, A2B_Device *dev,
                            A2B_NetworkMgr *net_mgr)
{
    if (!mgr || !dev || !net_mgr) return;

    mgr->dev = dev;
    mgr->net_mgr = net_mgr;
    mgr->status = A2B_DISC_STATUS_NOT_STARTED;
    mgr->current_node_id = 0;

    A2B_LOG_INFO(A2B_LOG_TAG_DISCOVERY, "Discovery manager initialized");
}

A2B_Result A2B_DiscoveryStart(A2B_DiscoveryMgr *mgr)
{
    if (!mgr) {
        return A2B_ERR_INVALID_PARAM;
    }

    if (mgr->status == A2B_DISC_STATUS_IN_PROGRESS) {
        A2B_LOG_WARN(A2B_LOG_TAG_DISCOVERY, "Discovery already in progress");
        return A2B_SUCCESS;
    }

    mgr->status = A2B_DISC_STATUS_IN_PROGRESS;
    mgr->current_node_id = 0;
    A2B_NetworkMgrClear(mgr->net_mgr);

    A2B_LOG_INFO(A2B_LOG_TAG_DISCOVERY, "Discovery started");

    return A2B_SUCCESS;
}

A2B_Result A2B_DiscoveryStep(A2B_DiscoveryMgr *mgr)
{
    if (!mgr) {
        return A2B_ERR_INVALID_PARAM;
    }

    if (mgr->status != A2B_DISC_STATUS_IN_PROGRESS) {
        A2B_LOG_ERROR(A2B_LOG_TAG_DISCOVERY, "Discovery not in progress");
        return A2B_ERR_STATE_INVALID;
    }

    if (mgr->current_node_id >= mgr->dev->config.max_nodes) {
        return A2B_DiscoveryComplete(mgr);
    }

    /* Simulate node discovery - in real implementation, this would:
     * 1. Send discovery broadcast
     * 2. Wait for response
     * 3. Parse node info
     * 4. Add to network
     */
    A2B_NodeInfo node = {
        .id = mgr->current_node_id,
        .address = (A2B_I2CAddr)(0x68 + mgr->current_node_id),
        .vendor_id = 0x0404,  /* Analog Devices */
        .product_id = 0x2428, /* AD2428 */
        .revision_id = 0x10,
        .state = A2B_NODE_STATE_ONLINE,
        .discovery_seq = mgr->current_node_id + 1
    };

    A2B_Result res = A2B_NetworkMgrAddNode(mgr->net_mgr, &node);

    if (res == A2B_SUCCESS) {
        A2B_LOG_INFO(A2B_LOG_TAG_DISCOVERY, "Discovered node %d (addr=0x%02X)",
                     mgr->current_node_id, node.address);
        mgr->current_node_id++;
    }

    return res;
}

A2B_Result A2B_DiscoveryComplete(A2B_DiscoveryMgr *mgr)
{
    if (!mgr) {
        return A2B_ERR_INVALID_PARAM;
    }

    mgr->status = A2B_DISC_STATUS_SUCCESS;

    A2B_LOG_INFO(A2B_LOG_TAG_DISCOVERY, "Discovery complete: %d nodes found",
                 mgr->current_node_id);

    return A2B_SUCCESS;
}

A2B_Result A2B_DiscoveryAbort(A2B_DiscoveryMgr *mgr)
{
    if (!mgr) {
        return A2B_ERR_INVALID_PARAM;
    }

    mgr->status = A2B_DISC_STATUS_FAILED;
    A2B_LOG_WARN(A2B_LOG_TAG_DISCOVERY, "Discovery aborted");

    return A2B_SUCCESS;
}

A2B_DiscoveryStatus A2B_DiscoveryGetStatus(const A2B_DiscoveryMgr *mgr)
{
    return mgr ? mgr->status : A2B_DISC_STATUS_FAILED;
}

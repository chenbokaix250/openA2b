#include "a2b_network.h"
#include "a2b_logger.h"
#include <stddef.h>

void A2B_NetworkMgrInit(A2B_NetworkMgr *mgr, A2B_Device *dev)
{
    if (!mgr || !dev) return;

    mgr->dev = dev;
    mgr->nodes = dev->nodes;
    mgr->max_nodes = dev->config.max_nodes;
    mgr->node_count = 0;

    A2B_LOG_INFO(A2B_LOG_TAG_NETWORK, "Network manager initialized (max_nodes=%d)",
                 mgr->max_nodes);
}

A2B_Result A2B_NetworkMgrAddNode(A2B_NetworkMgr *mgr, const A2B_NodeInfo *node)
{
    if (!mgr || !node) {
        return A2B_ERR_INVALID_PARAM;
    }

    if (mgr->node_count >= mgr->max_nodes) {
        A2B_LOG_ERROR(A2B_LOG_TAG_NETWORK, "Network full (max=%d)", mgr->max_nodes);
        return A2B_ERR_NO_MEM;
    }

    /* Check for duplicate address */
    for (int i = 0; i < mgr->node_count; i++) {
        if (mgr->nodes[i].address == node->address) {
            A2B_LOG_ERROR(A2B_LOG_TAG_NETWORK, "Duplicate node address 0x%02X",
                         node->address);
            return A2B_ERR_INVALID_PARAM;
        }
    }

    /* Add node */
    mgr->nodes[mgr->node_count] = *node;
    mgr->nodes[mgr->node_count].id = mgr->node_count;
    mgr->node_count++;

    A2B_LOG_INFO(A2B_LOG_TAG_NETWORK, "Node added: id=%d, addr=0x%02X, "
                 "vendor=0x%04X, product=0x%04X",
                 node->id, node->address, node->vendor_id, node->product_id);

    return A2B_SUCCESS;
}

A2B_Result A2B_NetworkMgrFindNodeById(const A2B_NetworkMgr *mgr, uint8_t id,
                                       A2B_NodeInfo **node)
{
    if (!mgr || !node) {
        return A2B_ERR_INVALID_PARAM;
    }

    for (int i = 0; i < mgr->node_count; i++) {
        if (mgr->nodes[i].id == id) {
            *node = &mgr->nodes[i];
            return A2B_SUCCESS;
        }
    }

    return A2B_ERR_NODE_NOT_FOUND;
}

A2B_Result A2B_NetworkMgrFindNodeByAddr(const A2B_NetworkMgr *mgr,
                                         A2B_I2CAddr addr, A2B_NodeInfo **node)
{
    if (!mgr || !node) {
        return A2B_ERR_INVALID_PARAM;
    }

    for (int i = 0; i < mgr->node_count; i++) {
        if (mgr->nodes[i].address == addr) {
            *node = &mgr->nodes[i];
            return A2B_SUCCESS;
        }
    }

    return A2B_ERR_NODE_NOT_FOUND;
}

A2B_Result A2B_NetworkMgrRemoveNode(A2B_NetworkMgr *mgr, uint8_t id)
{
    if (!mgr) {
        return A2B_ERR_INVALID_PARAM;
    }

    for (int i = 0; i < mgr->node_count; i++) {
        if (mgr->nodes[i].id == id) {
            /* Shift remaining nodes */
            for (int j = i; j < mgr->node_count - 1; j++) {
                mgr->nodes[j] = mgr->nodes[j + 1];
                mgr->nodes[j].id = j;
            }
            mgr->node_count--;

            A2B_LOG_INFO(A2B_LOG_TAG_NETWORK, "Node removed: id=%d", id);
            return A2B_SUCCESS;
        }
    }

    return A2B_ERR_NODE_NOT_FOUND;
}

uint8_t A2B_NetworkMgrGetNodeCount(const A2B_NetworkMgr *mgr)
{
    return mgr ? mgr->node_count : 0;
}

const A2B_NodeInfo *A2B_NetworkMgrGetNodes(const A2B_NetworkMgr *mgr)
{
    return mgr ? mgr->nodes : NULL;
}

void A2B_NetworkMgrClear(A2B_NetworkMgr *mgr)
{
    if (!mgr) return;

    mgr->node_count = 0;
    A2B_LOG_INFO(A2B_LOG_TAG_NETWORK, "Network cleared");
}
#ifndef A2B_NETWORK_H
#define A2B_NETWORK_H

#include "a2b_types.h"
#include "a2b_error.h"
#include "a2b_device.h"

/**
 * Network manager - manages node table
 */
typedef struct _A2B_NetworkMgr {
    A2B_Device       *dev;
    A2B_NodeInfo     *nodes;
    uint8_t           max_nodes;
    uint8_t           node_count;
} A2B_NetworkMgr;

/**
 * Initialize network manager
 */
void A2B_NetworkMgrInit(A2B_NetworkMgr *mgr, A2B_Device *dev);

/**
 * Add node to network
 */
A2B_Result A2B_NetworkMgrAddNode(A2B_NetworkMgr *mgr, const A2B_NodeInfo *node);

/**
 * Find node by ID
 */
A2B_Result A2B_NetworkMgrFindNodeById(const A2B_NetworkMgr *mgr, uint8_t id,
                                       A2B_NodeInfo **node);

/**
 * Find node by I2C address
 */
A2B_Result A2B_NetworkMgrFindNodeByAddr(const A2B_NetworkMgr *mgr,
                                          A2B_I2CAddr addr, A2B_NodeInfo **node);

/**
 * Remove node from network
 */
A2B_Result A2B_NetworkMgrRemoveNode(A2B_NetworkMgr *mgr, uint8_t id);

/**
 * Get node count
 */
uint8_t A2B_NetworkMgrGetNodeCount(const A2B_NetworkMgr *mgr);

/**
 * Get all nodes
 */
const A2B_NodeInfo *A2B_NetworkMgrGetNodes(const A2B_NetworkMgr *mgr);

/**
 * Clear all nodes
 */
void A2B_NetworkMgrClear(A2B_NetworkMgr *mgr);

#endif /* A2B_NETWORK_H */
#ifndef A2B_DIAGNOSTIC_H
#define A2B_DIAGNOSTIC_H

#include "a2b_types.h"
#include "a2b_error.h"
#include "a2b_device.h"
#include "a2b_network.h"

/* Diagnostic event types */
typedef enum {
    A2B_DIAG_EVENT_NONE = 0,
    A2B_DIAG_EVENT_I2C_ERROR,
    A2B_DIAG_EVENT_CRC_ERROR,
    A2B_DIAG_EVENT_FRAME_ERROR,
    A2B_DIAG_EVENT_PLL_ERROR,
    A2B_DIAG_EVENT_NODE_ERROR
} A2B_DiagEvent;

/* Diagnostic record */
typedef struct _A2B_DiagRecord {
    uint32_t        timestamp;
    A2B_DiagEvent   event;
    uint8_t         node_id;
    uint8_t         detail;
} A2B_DiagRecord;

/* Diagnostic manager context */
typedef struct _A2B_DiagMgr {
    A2B_Device       *dev;
    A2B_NetworkMgr   *net_mgr;
    A2B_DiagRecord   *records;
    uint16_t         max_records;
    uint16_t         record_count;
    uint16_t         next_record;
} A2B_DiagMgr;

/**
 * Initialize diagnostic manager
 */
void A2B_DiagMgrInit(A2B_DiagMgr *mgr, A2B_Device *dev,
                       A2B_NetworkMgr *net_mgr, uint16_t max_records);

/**
 * Log a diagnostic event
 */
void A2B_DiagMgrLogEvent(A2B_DiagMgr *mgr, A2B_DiagEvent event,
                           uint8_t node_id, uint8_t detail);

/**
 * Get diagnostic record count
 */
uint16_t A2B_DiagMgrGetRecordCount(const A2B_DiagMgr *mgr);

/**
 * Get latest record
 */
A2B_Result A2B_DiagMgrGetLatestRecord(const A2B_DiagMgr *mgr,
                                        A2B_DiagRecord *record);

/**
 * Clear all records
 */
void A2B_DiagMgrClearRecords(A2B_DiagMgr *mgr);

#endif /* A2B_DIAGNOSTIC_H */
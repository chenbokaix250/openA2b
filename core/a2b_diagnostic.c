#include "a2b_diagnostic.h"
#include "a2b_logger.h"
#include <stddef.h>

void A2B_DiagMgrInit(A2B_DiagMgr *mgr, A2B_Device *dev,
                       A2B_NetworkMgr *net_mgr, uint16_t max_records)
{
    if (!mgr || !dev || !net_mgr) return;

    mgr->dev = dev;
    mgr->net_mgr = net_mgr;
    mgr->max_records = max_records;
    mgr->record_count = 0;
    mgr->next_record = 0;
    mgr->records = NULL;

    A2B_LOG_INFO(A2B_LOG_TAG_MAIN, "Diagnostic manager initialized (max=%d)",
                 max_records);
}

void A2B_DiagMgrLogEvent(A2B_DiagMgr *mgr, A2B_DiagEvent event,
                           uint8_t node_id, uint8_t detail)
{
    if (!mgr || !mgr->records) return;

    A2B_DiagRecord *rec = &mgr->records[mgr->next_record];
    rec->timestamp = 0;  /* Would use platform timer in real impl */
    rec->event = event;
    rec->node_id = node_id;
    rec->detail = detail;

    mgr->next_record = (mgr->next_record + 1) % mgr->max_records;
    if (mgr->record_count < mgr->max_records) {
        mgr->record_count++;
    }

    A2B_LOG_WARN(A2B_LOG_TAG_MAIN, "Diag event: type=%d, node=%d", event, node_id);
}

uint16_t A2B_DiagMgrGetRecordCount(const A2B_DiagMgr *mgr)
{
    return mgr ? mgr->record_count : 0;
}

A2B_Result A2B_DiagMgrGetLatestRecord(const A2B_DiagMgr *mgr,
                                        A2B_DiagRecord *record)
{
    if (!mgr || !record) {
        return A2B_ERR_INVALID_PARAM;
    }

    if (mgr->record_count == 0) {
        return A2B_ERR_GENERIC;
    }

    uint16_t idx = (mgr->next_record == 0) ?
                    (mgr->max_records - 1) : (mgr->next_record - 1);
    *record = mgr->records[idx];

    return A2B_SUCCESS;
}

void A2B_DiagMgrClearRecords(A2B_DiagMgr *mgr)
{
    if (!mgr) return;

    mgr->record_count = 0;
    mgr->next_record = 0;

    A2B_LOG_INFO(A2B_LOG_TAG_MAIN, "Diagnostic records cleared");
}
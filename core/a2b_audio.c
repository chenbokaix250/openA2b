#include "a2b_audio.h"
#include "a2b_logger.h"
#include <stddef.h>

void A2B_AudioMgrInit(A2B_AudioMgr *mgr, A2B_Device *dev, A2B_NetworkMgr *net_mgr,
                        uint8_t max_routes)
{
    if (!mgr || !dev || !net_mgr) return;

    mgr->dev = dev;
    mgr->net_mgr = net_mgr;
    mgr->max_routes = max_routes;
    mgr->route_count = 0;
    mgr->routes = NULL;

    A2B_LOG_INFO(A2B_LOG_TAG_ROUTE, "Audio manager initialized (max_routes=%d)",
                 max_routes);
}

A2B_Result A2B_AudioMgrSetRoute(A2B_AudioMgr *mgr, const A2B_AudioRoute *route)
{
    if (!mgr || !route) {
        return A2B_ERR_INVALID_PARAM;
    }

    if (mgr->route_count >= mgr->max_routes) {
        A2B_LOG_ERROR(A2B_LOG_TAG_ROUTE, "Route table full (max=%d)", mgr->max_routes);
        return A2B_ERR_NO_MEM;
    }

    A2B_AudioRoute *r = &mgr->routes[mgr->route_count];
    r->src_node = route->src_node;
    r->dst_node = route->dst_node;
    r->src_slot = route->src_slot;
    r->dst_slot = route->dst_slot;
    r->num_slots = route->num_slots;
    r->direction = route->direction;
    r->format = route->format;
    r->enabled = false;

    uint8_t route_id = mgr->route_count;
    mgr->route_count++;

    A2B_LOG_INFO(A2B_LOG_TAG_ROUTE, "Route configured: node%d->node%d, slot%d->slot%d",
                 r->src_node, r->dst_node, r->src_slot, r->dst_slot);

    return A2B_SUCCESS;
}

A2B_Result A2B_AudioMgrEnableRoute(A2B_AudioMgr *mgr, uint8_t route_id)
{
    if (!mgr) {
        return A2B_ERR_INVALID_PARAM;
    }

    if (route_id >= mgr->route_count) {
        return A2B_ERR_INVALID_PARAM;
    }

    mgr->routes[route_id].enabled = true;

    A2B_LOG_INFO(A2B_LOG_TAG_ROUTE, "Route %d enabled", route_id);

    return A2B_SUCCESS;
}

A2B_Result A2B_AudioMgrDisableRoute(A2B_AudioMgr *mgr, uint8_t route_id)
{
    if (!mgr) {
        return A2B_ERR_INVALID_PARAM;
    }

    if (route_id >= mgr->route_count) {
        return A2B_ERR_INVALID_PARAM;
    }

    mgr->routes[route_id].enabled = false;

    A2B_LOG_INFO(A2B_LOG_TAG_ROUTE, "Route %d disabled", route_id);

    return A2B_SUCCESS;
}

uint8_t A2B_AudioMgrGetRouteCount(const A2B_AudioMgr *mgr)
{
    return mgr ? mgr->route_count : 0;
}

#ifndef A2B_AUDIO_H
#define A2B_AUDIO_H

#include "a2b_types.h"
#include "a2b_error.h"
#include "a2b_device.h"
#include "a2b_network.h"

/* Audio route direction */
typedef enum {
    A2B_AUDIO_DIR_UPSTREAM = 0,   /* From slave to master */
    A2B_AUDIO_DIR_DOWNSTREAM       /* From master to slave */
} A2B_AudioDir;

/* Audio format */
typedef enum {
    A2B_AUDIO_FMT_I2S = 0,
    A2B_AUDIO_FMT_TDM
} A2B_AudioFmt;

/* Audio route */
typedef struct _A2B_AudioRoute {
    uint8_t             src_node;
    uint8_t             dst_node;
    uint8_t             src_slot;
    uint8_t             dst_slot;
    uint8_t             num_slots;
    A2B_AudioDir        direction;
    A2B_AudioFmt        format;
    bool                enabled;
} A2B_AudioRoute;

/* Audio manager context */
typedef struct _A2B_AudioMgr {
    A2B_Device         *dev;
    A2B_NetworkMgr     *net_mgr;
    A2B_AudioRoute     *routes;
    uint8_t             max_routes;
    uint8_t             route_count;
} A2B_AudioMgr;

/**
 * Initialize audio manager
 */
void A2B_AudioMgrInit(A2B_AudioMgr *mgr, A2B_Device *dev, A2B_NetworkMgr *net_mgr,
                        uint8_t max_routes);

/**
 * Configure audio route
 */
A2B_Result A2B_AudioMgrSetRoute(A2B_AudioMgr *mgr, const A2B_AudioRoute *route);

/**
 * Enable audio route
 */
A2B_Result A2B_AudioMgrEnableRoute(A2B_AudioMgr *mgr, uint8_t route_id);

/**
 * Disable audio route
 */
A2B_Result A2B_AudioMgrDisableRoute(A2B_AudioMgr *mgr, uint8_t route_id);

/**
 * Get route count
 */
uint8_t A2B_AudioMgrGetRouteCount(const A2B_AudioMgr *mgr);

#endif /* A2B_AUDIO_H */

# OpenA2B API Reference

## Core Functions

### Device Management

#### A2B_DeviceInit
```c
void A2B_DeviceInit(A2B_Device *dev, const A2B_Config *config);
```
Initialize an A2B device instance.

**Parameters:**
- `dev` - Device structure to initialize
- `config` - Configuration parameters (or NULL for defaults)

#### A2B_DeviceDeinit
```c
void A2B_DeviceDeinit(A2B_Device *dev);
```
Deinitialize and release device resources.

---

### Network Management

#### A2B_NetworkMgrInit
```c
void A2B_NetworkMgrInit(A2B_NetworkMgr *mgr, A2B_Device *dev);
```
Initialize the network manager.

#### A2B_NetworkMgrAddNode
```c
A2B_Result A2B_NetworkMgrAddNode(A2B_NetworkMgr *mgr, const A2B_NodeInfo *node);
```
Add a node to the network.

---

### Discovery

#### A2B_DiscoveryStart
```c
A2B_Result A2B_DiscoveryStart(A2B_DiscoveryMgr *mgr);
```
Start the discovery process.

#### A2B_DiscoveryStep
```c
A2B_Result A2B_DiscoveryStep(A2B_DiscoveryMgr *mgr);
```
Execute single discovery step (for async usage).

#### A2B_DiscoveryComplete
```c
A2B_Result A2B_DiscoveryComplete(A2B_DiscoveryMgr *mgr);
```
Complete discovery and finalize network.

---

### Audio Routing

#### A2B_AudioMgrSetRoute
```c
A2B_Result A2B_AudioMgrSetRoute(A2B_AudioMgr *mgr, const A2B_AudioRoute *route);
```
Configure an audio route.

#### A2B_AudioMgrEnableRoute
```c
A2B_Result A2B_AudioMgrEnableRoute(A2B_AudioMgr *mgr, uint8_t route_id);
```
Enable a configured audio route.

---

### Clock Management

#### A2B_ClockMgrConfigure
```c
A2B_Result A2B_ClockMgrConfigure(A2B_ClockMgr *mgr, const A2B_ClockConfig *config);
```
Configure clock source and frequency.

#### A2B_ClockMgrStartPLL
```c
A2B_Result A2B_ClockMgrStartPLL(A2B_ClockMgr *mgr);
```
Start PLL locking sequence.

#### A2B_ClockMgrWaitLock
```c
A2B_Result A2B_ClockMgrWaitLock(A2B_ClockMgr *mgr, uint32_t timeout_ms);
```
Wait for PLL to achieve lock.

---

### Diagnostics

#### A2B_DiagMgrLogEvent
```c
void A2B_DiagMgrLogEvent(A2B_DiagMgr *mgr, A2B_DiagEvent event,
                           uint8_t node_id, uint8_t detail);
```
Log a diagnostic event.

#### A2B_DiagMgrGetLatestRecord
```c
A2B_Result A2B_DiagMgrGetLatestRecord(const A2B_DiagMgr *mgr,
                                        A2B_DiagRecord *record);
```
Get most recent diagnostic record.

---

### Recovery

#### A2B_RecoveryMgrRecover
```c
A2B_Result A2B_RecoveryMgrRecover(A2B_RecoveryMgr *mgr,
                                     A2B_RecoveryStrategy strategy,
                                     uint8_t node_id);
```
Attempt recovery with specified strategy.

#### A2B_RecoveryMgrAbort
```c
A2B_Result A2B_RecoveryMgrAbort(A2B_RecoveryMgr *mgr);
```
Abort ongoing recovery.

---

## Error Codes

| Code | Value | Description |
|------|-------|-------------|
| A2B_SUCCESS | 0 | Success |
| A2B_ERR_GENERIC | -1 | Generic error |
| A2B_ERR_INVALID_PARAM | -2 | Invalid parameter |
| A2B_ERR_NO_MEM | -3 | Out of memory |
| A2B_ERR_TIMEOUT | -4 | Operation timeout |
| A2B_ERR_I2C_NACK | -5 | I2C NACK received |
| A2B_ERR_NODE_NOT_FOUND | -7 | Node not found |
| A2B_ERR_PLL_NOT_LOCKED | -8 | PLL not locked |
| A2B_ERR_DISCOVERY_FAILED | -9 | Discovery failed |
| A2B_ERR_LINK_DOWN | -10 | Link is down |
| A2B_ERR_RECOVERY_FAILED | -11 | Recovery failed |

---

## Logging

### Log Levels

```c
typedef enum {
    A2B_LOG_NONE = 0,
    A2B_LOG_ERROR,
    A2B_LOG_WARN,
    A2B_LOG_INFO,
    A2B_LOG_DEBUG,
    A2B_LOG_TRACE
} A2B_LogLevel;
```

### Log Tags

| Tag | Module |
|-----|--------|
| MAIN | Main framework |
| DISCOVERY | Discovery manager |
| NETWORK | Network manager |
| ROUTE | Audio routing |
| CLOCK | Clock management |
| IRQ | Interrupt handling |
| I2C | I2C bus layer |

### Compile-Time Control

```c
#define A2B_LOG_LEVEL A2B_LOG_INFO
```

Set to desired level at compile time to strip higher-level logs in release builds.

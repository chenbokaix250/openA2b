# OpenA2B Architecture

## Overview

OpenA2B is an A2B (Automotive Audio Bus) master stack following Linux Kernel Driver design patterns.

## Design Principles

1. **No Globals**: All state is in device structures
2. **Ops-Based**: Function pointers for platform abstraction
3. **Device-Centric**: Everything hangs off A2B_Device
4. **Testable**: Mock hardware enables full CI

## Layer Architecture

```
┌─────────────────────────────────────────────────────────────┐
│                      Application Layer                      │
└─────────────────────────────────────────────────────────────┘
                              │
                              ▼
┌─────────────────────────────────────────────────────────────┐
│                     A2B Framework Core                      │
│  ┌─────────────┐ ┌─────────────┐ ┌─────────────┐ ┌───────┐ │
│  │  Discovery  │ │   Audio     │ │   Clock     │ │  IRQ  │ │
│  │  Manager    │ │   Manager   │ │   Manager   │ │  Mgr  │ │
│  └─────────────┘ └─────────────┘ └─────────────┘ └───────┘ │
│  ┌─────────────┐ ┌─────────────┐ ┌─────────────┐ ┌───────┐ │
│  │   Network   │ │   State     │ │ Diagnostic  │ │Remote │ │
│  │   Manager   │ │   Machine   │ │   Manager   │ │Register│
│  └─────────────┘ └─────────────┘ └─────────────┘ └───────┘ │
└─────────────────────────────────────────────────────────────┘
                              │
                              ▼
┌─────────────────────────────────────────────────────────────┐
│                     Register Abstraction                     │
│  ┌─────────────────────────────────────────────────────┐  │
│  │         AD2428 Register Database (ad2428_reg.h)      │  │
│  └─────────────────────────────────────────────────────┘  │
└─────────────────────────────────────────────────────────────┘
                              │
                              ▼
┌─────────────────────────────────────────────────────────────┐
│                      Bus Interface Layer                     │
│              (I2C / SPI - Platform Implementation)           │
└─────────────────────────────────────────────────────────────┘
                              │
                              ▼
┌─────────────────────────────────────────────────────────────┐
│                     Platform HAL                             │
│        (Linux / STM32 / S32K / TC397 / etc.)               │
└─────────────────────────────────────────────────────────────┘
```

## Key Structures

### A2B_Device
Main device structure containing all subsystem managers:
```c
typedef struct _A2B_Device {
    A2B_I2C_HANDLE     bus;
    A2B_Config         config;
    A2B_State          state;
    A2B_ClockMode      clock_mode;
} A2B_Device;
```

### A2B_NetworkMgr
Network topology management:
```c
typedef struct _A2B_NetworkMgr {
    A2B_Device     *dev;
    A2B_NodeInfo   nodes[A2B_MAX_NODES];
    uint8_t        node_count;
} A2B_NetworkMgr;
```

## State Machine

```
     ┌──────────┐
     │   INIT   │
     └────┬─────┘
          ▼
     ┌──────────┐
     │   IDLE   │
     └────┬─────┘
          ▼
   ┌─────────────┐
   │WAIT_POWER   │
   └────┬────────┘
        ▼
   ┌─────────────┐
   │  WAIT_PLL   │
   └────┬────────┘
        ▼
   ┌─────────────┐
   │ DISCOVERY   │──────────┐
   └────┬────────┘          │
        ▼                   │
   ┌─────────────┐          │
   │CONFIG_NODE  │          │
   └────┬────────┘          │
        ▼                   │
   ┌─────────────┐          │
   │CONFIG_SLOT  │          │
   └────┬────────┘          │
        ▼                   │
   ┌─────────────┐          │
   │CONFIG_ROUTE │          │
   └────┬────────┘          │
        ▼                   │
   ┌─────────────┐          │
   │  RUNNING    │◄─────────┘
   └──────┬──────┘
          │ link down
          ▼
   ┌─────────────┐
   │LINK_DOWN    │
   └──────┬──────┘
          │ recovery
          ▼
   ┌─────────────┐
   │ RECOVERY    │──┐
   └─────────────┘  │
          │          │
          └──────────┘
```

## Testing Architecture

```
┌─────────────────────────────────────────┐
│              Unity Framework             │
│  ┌─────────────────────────────────┐   │
│  │         Test Suites              │   │
│  │  test_audio  test_clock         │   │
│  │  test_discovery  test_network   │   │
│  └─────────────────────────────────┘   │
└─────────────────────────────────────────┘
                    │
                    ▼
┌─────────────────────────────────────────┐
│              Mock Hardware               │
│  ┌─────────────┐  ┌─────────────────┐   │
│  │  Mock AD2428 │  │   Mock I2C     │   │
│  └─────────────┘  └─────────────────┘   │
└─────────────────────────────────────────┘
                    │
                    ▼
┌─────────────────────────────────────────┐
│              Build System               │
│         (CMake + GitHub Actions)         │
└─────────────────────────────────────────┘
```

## Platform Abstraction

The HAL layer provides platform-independent interfaces:

| Interface | Purpose |
|-----------|---------|
| `A2B_I2C_Ops` | I2C read/write/burst operations |
| `A2B_Timer_Ops` | Timer init/start/stop operations |

Platform implementations are in `hal/platform/<platform>/`.

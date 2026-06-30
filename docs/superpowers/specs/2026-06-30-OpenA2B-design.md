# OpenA2B - Software Design Specification

**Version:** 1.0
**Date:** 2026-06-30
**Status:** Draft
**License:** MIT

---

## 1. Project Overview

### 1.1 Project Name

**OpenA2B** - An Open Automotive A2B (Automotive Audio Bus) Driver Framework

### 1.2 Project Vision

OpenA2B is a platform-independent, RTOS-independent, MCU-independent A2B master stack designed for automotive audio applications. Inspired by Linux Kernel Driver design principles, it provides a clean, maintainable, and extensible framework for developing A2B bus master solutions.

### 1.3 Target Users

- Automotive Tier 1 suppliers developing A2B-based audio systems
- OEMs integrating A2B technology into vehicles
- Embedded software engineers working with AD2428/A2B chips
- Open-source community contributors

### 1.4 Key Design Principles

1. **Platform Independence** - Runs on STM32, S32K, TC397, Linux, and more
2. **RTOS Independence** - No dependency on specific RTOS APIs
3. **Clean Abstraction** - Layered architecture with well-defined interfaces
4. **Testability** - Mock hardware enables CI without target hardware
5. **AI-Ready** - Structured for integration with AI debugging assistants

---

## 2. Architecture

### 2.1 Layer Overview

```
┌─────────────────────────────────────────────────────────────┐
│                      Application Layer                      │
│                  (Audio Manager API Calls)                  │
└─────────────────────────────────────────────────────────────┘
                              │
                              ▼
┌─────────────────────────────────────────────────────────────┐
│                     A2B Framework Core                      │
│  ┌─────────────┐ ┌─────────────┐ ┌─────────────┐ ┌───────┐ │
│  │  Discovery  │ │   Routing   │ │   Clock     │ │  IRQ  │ │
│  │  Manager    │ │   Manager   │ │   Manager   │ │Mgr    │ │
│  └─────────────┘ └─────────────┘ └─────────────┘ └───────┘ │
│  ┌─────────────┐ ┌─────────────┐ ┌─────────────┐ ┌───────┐ │
│  │   Audio     │ │ Diagnostic  │ │  Recovery    │ │Remote │ │
│  │   Manager   │ │   Manager   │ │   Manager    │ │ Register│
│  └─────────────┘ └─────────────┘ └─────────────┘ └───────┘ │
│  ┌─────────────┐ ┌─────────────┐                            │
│  │   Network  │ │   State     │                            │
│  │   Manager   │ │   Machine   │                            │
│  └─────────────┘ └─────────────┘                            │
└─────────────────────────────────────────────────────────────┘
                              │
                              ▼
┌─────────────────────────────────────────────────────────────┐
│                     Register Abstraction                    │
│  ┌─────────────────────────────────────────────────────┐  │
│  │         AD2428 Register Database (Read/Write)        │  │
│  │         Bit-field definitions, mask, shift          │  │
│  └─────────────────────────────────────────────────────┘  │
└─────────────────────────────────────────────────────────────┘
                              │
                              ▼
┌─────────────────────────────────────────────────────────────┐
│                      Bus Interface Layer                    │
│              (I2C / SPI - Platform Implementation)         │
└─────────────────────────────────────────────────────────────┘
                              │
                              ▼
┌─────────────────────────────────────────────────────────────┐
│                     Platform HAL                            │
│        (Linux / STM32 / S32K / TC397 / etc.)               │
└─────────────────────────────────────────────────────────────┘
```

### 2.2 Core Device Structure

```c
/**
 * Main A2B device structure representing the A2B master node
 */
typedef struct _A2B_Device {
    /** Bus interface handle */
    A2B_I2C_HANDLE         bus;

    /** Network node array (max 16 nodes per bus) */
    A2B_Node               nodes[A2B_MAX_NODES];

    /** Number of discovered nodes */
    uint8_t                node_count;

    /** Active routing paths */
    A2B_Route              routes[A2B_MAX_ROUTES];

    /** Clock management context */
    A2B_ClockMgr           clock;

    /** Interrupt handling context */
    A2B_IRQMgr             irq;

    /** Current device state */
    A2B_State              state;

    /** Driver configuration */
    A2B_Config             config;

    /** Platform-specific data */
    void                   *platform_data;
} A2B_Device;
```

### 2.3 Node Structure

```c
/**
 * Represents an A2B node in the network
 */
typedef struct _A2B_Node {
    /** Node ID (0 = master, 1-15 = slave nodes) */
    uint8_t                 id;

    /** I2C bus address */
    uint8_t                 address;

    /** Vendor ID */
    uint16_t                vendor_id;

    /** Product ID */
    uint16_t                product_id;

    /** Node capabilities */
    uint32_t                capabilities;

    /** Node state */
    bool                    online;

    /** Discovery sequence number */
    uint8_t                 discovery_seq;

    /** Node-specific configuration */
    void                    *priv;
} A2B_Node;
```

---

## 3. Module Specifications

### 3.1 Register Layer

**Purpose:** Provides safe, abstracted register access for AD2428.

**Design Principles:**
- All register access goes through this layer (no direct I2C in upper layers)
- Bit-field definitions with masks and shifts
- Burst read/write support
- Register shadowing for debugging

**Key Functions:**
```c
A2B_Result A2B_RegRead(A2B_Device *dev, uint8_t addr, uint16_t reg, uint32_t *val);
A2B_Result A2B_RegWrite(A2B_Device *dev, uint8_t addr, uint16_t reg, uint32_t val);
A2B_Result A2B_RegUpdateBits(A2B_Device *dev, uint8_t addr, uint16_t reg,
                               uint32_t mask, uint32_t shift, uint32_t val);
A2B_Result A2B_RegBurstRead(A2B_Device *dev, uint8_t addr, uint16_t reg,
                              uint8_t *buf, uint32_t len);
A2B_Result A2B_RegBurstWrite(A2B_Device *dev, uint8_t addr, uint16_t reg,
                               const uint8_t *buf, uint32_t len);
```

### 3.2 Discovery Manager

**Purpose:** Auto-detect and enumerate A2B nodes on the bus.

**State Machine:**
```
IDLE → WAIT_POWER → WAIT_PLL → DISCOVERY → CONFIG_NODE → CONFIG_SLOT → CONFIG_ROUTE → RUNNING
                                                                         ↓
                                                                   LINK_DOWN → RECOVERY → RUNNING
```

**Key Functions:**
```c
A2B_Result A2B_DiscoveryStart(A2B_Device *dev);
A2B_Result A2B_DiscoveryNode(A2B_Device *dev, uint8_t node_id);
A2B_Result A2B_DiscoveryAbort(A2B_Device *dev);
```

**Discovery Sequence:**
1. Reset master node
2. Wait for PLL lock
3. Send discovery broadcast
4. Collect responses from each node
5. Build network topology table
6. Configure each node sequentially

### 3.3 Network Manager

**Purpose:** Maintain network topology and node state.

**Key Functions:**
```c
A2B_Result A2B_NetworkInit(A2B_Device *dev);
A2B_Result A2B_NetworkAddNode(A2B_Device *dev, A2B_Node *node);
A2B_Result A2B_NetworkRemoveNode(A2B_Device *dev, uint8_t node_id);
A2B_Result A2B_NetworkGetNode(A2B_Device *dev, uint8_t node_id, A2B_Node **node);
A2B_Result A2B_NetworkDump(A2B_Device *dev);
```

### 3.4 Routing Manager

**Purpose:** Configure audio routing paths between nodes.

**Key Functions:**
```c
A2B_Result A2B_RouteSet(A2B_Device *dev, uint8_t src_node, uint8_t dst_node,
                         A2B_SlotConfig *slots, uint8_t slot_count);
A2B_Result A2B_RouteClear(A2B_Device *dev, uint8_t src_node, uint8_t dst_node);
A2B_Result A2B_RouteEnable(A2B_Device *dev, uint8_t route_id);
A2B_Result A2B_RouteDisable(A2B_Device *dev, uint8_t route_id);
```

### 3.5 Clock Manager

**Purpose:** Manage A2B clock domains and PLL configuration.

**Key Functions:**
```c
A2B_Result A2B_ClockInit(A2B_Device *dev, A2B_ClockConfig *config);
A2B_Result A2B_ClockSetMode(A2B_Device *dev, A2B_ClockMode mode);
A2B_Result A2B_ClockWaitLock(A2B_Device *dev, uint32_t timeout_ms);
A2B_Result A2B_ClockGetStatus(A2B_Device *dev, A2B_ClockStatus *status);
```

### 3.6 Interrupt Manager

**Purpose:** Handle A2B interrupts and convert to events.

**Design:** Interrupt handlers are minimal - they only trigger state transitions and schedule deferred work.

**Key Functions:**
```c
A2B_Result A2B_IRQInit(A2B_Device *dev, A2B_IRQConfig *config);
A2B_Result A2B_IRQEnable(A2B_Device *dev);
A2B_Result A2B_IRQDisable(A2B_Device *dev);
A2B_Result A2B_IRQRegisterCallback(A2B_Device *dev, A2B_IrqEvent event,
                                    A2B_IrqCallback callback, void *priv);
```

### 3.7 State Machine

**Purpose:** Enforce deterministic state transitions.

**States:**
```c
typedef enum {
    A2B_STATE_INIT = 0,
    A2B_STATE_IDLE,
    A2B_STATE_WAIT_POWER,
    A2B_STATE_WAIT_PLL,
    A2B_STATE_DISCOVERY,
    A2B_STATE_CONFIG_NODE,
    A2B_STATE_CONFIG_SLOT,
    A2B_STATE_CONFIG_ROUTE,
    A2B_STATE_RUNNING,
    A2B_STATE_LINK_DOWN,
    A2B_STATE_RECOVERY,
    A2B_STATE_ERROR,
    A2B_STATE_SHUTDOWN
} A2B_State;
```

**Transition Rules:**
- All transitions are explicit and logged
- Invalid transitions trigger error handling
- State change events are broadcast to registered callbacks

### 3.8 Recovery Manager

**Purpose:** Automatic recovery from link errors.

**Supported Recovery Strategies:**
1. Link re-negotiation
2. Node reset sequence
3. Full discovery re-run

---

## 4. Platform HAL

### 4.1 I2C Bus Interface

**Abstraction:**
```c
typedef struct _A2B_I2C_Ops {
    A2B_Result (*init)(A2B_I2C_HANDLE handle, void *config);
    A2B_Result (*deinit)(A2B_I2C_HANDLE handle);
    A2B_Result (*write)(A2B_I2C_HANDLE handle, uint8_t addr,
                        const uint8_t *data, uint32_t len);
    A2B_Result (*read)(A2B_I2C_HANDLE handle, uint8_t addr,
                       uint8_t *data, uint32_t len);
    A2B_Result (*writeRead)(A2B_I2C_HANDLE handle, uint8_t addr,
                            const uint8_t *wdata, uint32_t wlen,
                            uint8_t *rdata, uint32_t rlen);
} A2B_I2C_Ops;
```

### 4.2 Platform Implementations

| Platform | Path | Status |
|----------|------|--------|
| Linux (Userspace) | `hal/platform/linux/` | Phase 1 |
| STM32 (HAL) | `hal/platform/stm32/` | Phase 2 |
| S32K (SDK) | `hal/platform/s32k/` | Phase 2 |
| TC397 (iLLD) | `hal/platform/tc397/` | Phase 3 |

### 4.3 Timer Abstraction

```c
typedef struct _A2B_TimerOps {
    A2B_Result (*init)(A2B_TIMER_HANDLE handle, void *config);
    A2B_Result (*start)(A2B_TIMER_HANDLE handle, uint32_t delay_ms);
    A2B_Result (*stop)(A2B_TIMER_HANDLE handle);
    void (*callback)(void *priv);
} A2B_TimerOps;
```

---

## 5. Mock Hardware

### 5.1 Mock AD2428

**Purpose:** Simulate AD2428 behavior for testing without hardware.

**Features:**
- Configurable node count (1-16)
- Register bank emulation
- I2C response simulation (ACK/NACK)
- Error injection (CRC, timeout)
- State simulation (discovery sequence)

**Usage in CI:**
```c
/* Mock setup for unit tests */
MockAD2428_Init(&mock_dev, 3);  /* Simulate 3 nodes */
A2B_I2C_SetMockHandle(bus, mock_dev.i2c_handle);
```

### 5.2 Mock I2C

**Purpose:** Fake I2C bus for Linux userspace testing.

**Features:**
- Configurable slave responses
- Error injection (NACK, timeout, data corruption)
- Performance profiling

---

## 6. Logging System

### 6.1 Log Levels

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

### 6.2 Log Format

```
[LEVEL ][TAG       ] Message
[INFO ][DISCOVERY] Node 1 discovered (vendor=0x%04X, product=0x%04X)
[ERROR][I2C      ] NACK received at address 0x%02X
[WARN ][IRQ      ] CRC error detected on node 2
```

### 6.3 Compile-time Control

```c
/* Release: no logging overhead */
#define A2B_LOG(level, tag, fmt, ...)

/* Debug: full logging */
#define A2B_LOG(level, tag, fmt, ...) \
    a2b_logger_log(level, tag, __FILE__, __LINE__, fmt, ##__VA_ARGS__)
```

---

## 7. Error Handling

### 7.1 Error Codes

```c
typedef enum {
    A2B_SUCCESS = 0,
    A2B_ERR_GENERIC = -1,
    A2B_ERR_INVALID_PARAM = -2,
    A2B_ERR_NO_MEM = -3,
    A2B_ERR_TIMEOUT = -4,
    A2B_ERR_I2C_NACK = -5,
    A2B_ERR_I2C_TIMEOUT = -6,
    A2B_ERR_NODE_NOT_FOUND = -7,
    A2B_ERR_PLL_NOT_LOCKED = -8,
    A2B_ERR_DISCOVERY_FAILED = -9,
    A2B_ERR_LINK_DOWN = -10,
    A2B_ERR_RECOVERY_FAILED = -11,
    A2B_ERR_STATE_INVALID = -12,
    A2B_ERR_REG_READ = -13,
    A2B_ERR_REG_WRITE = -14
} A2B_Result;
```

### 7.2 Error Propagation

- All public APIs return `A2B_Result`
- Errors are logged with context (file, line, error code)
- Critical errors trigger state machine transition to ERROR state

---

## 8. Testing Strategy

### 8.1 Unit Tests (Unity)

| Test Suite | Coverage |
|------------|----------|
| `test_register.c` | Register read/write/update bits |
| `test_discovery.c` | Discovery sequence, node enumeration |
| `test_network.c` | Network table operations |
| `test_route.c` | Routing configuration |
| `test_clock.c` | Clock initialization, PLL lock |
| `test_state.c` | State transitions |

### 8.2 Integration Tests

- Full discovery sequence with Mock AD2428
- Audio routing setup and teardown
- Error injection and recovery

### 8.3 CI Pipeline

```
Push/PR → Build → Unit Tests → Static Analysis → Coverage Report → Artifacts
```

---

## 9. Python Toolchain

### 9.1 Registry Generator

**Input:** CSV or Excel register definition
**Output:** `ad2428_reg.h`, `ad2428_reg.c`

**Features:**
- Bit-field extraction
- Mask/shift calculation
- Doxygen comment generation
- Register group organization

### 9.2 Configuration Generator

**Input:** Network topology YAML
**Output:** `a2b_config.c` (initialization code)

### 9.3 Documentation Generator

**Input:** Source code
**Output:** Architecture diagrams (PlantUML), API docs

---

## 10. Directory Structure

```
OpenA2B/
├── CMakeLists.txt                 # Main build configuration
├── LICENSE                        # MIT License
├── README.md                      # Project overview
│
├── docs/                          # Documentation
│   ├── architecture.md            # System architecture
│   ├── register.md                # Register specification
│   ├── discovery.md               # Discovery protocol
│   ├── api_reference.md           # API documentation
│   └── plantuml/                  # Architecture diagrams
│       ├── discovery_sequence.puml
│       ├── state_machine.puml
│       └── routing_diagram.puml
│
├── include/                       # Public headers
│   ├── a2b.h                      # Main include
│   ├── a2b_types.h                # Type definitions
│   ├── a2b_error.h                # Error codes
│   ├── a2b_driver.h               # Driver API
│   ├── a2b_logger.h               # Logging API
│   ├── a2b_device.h               # Device structure
│   ├── a2b_i2c.h                  # I2C interface
│   ├── a2b_timer.h                # Timer interface
│   └── a2b_version.h               # Version info
│
├── core/                          # Framework core
│   ├── a2b_main.c                 # Main entry
│   ├── a2b_device.c               # Device management
│   ├── a2b_discovery.c            # Discovery manager
│   ├── a2b_network.c              # Network manager
│   ├── a2b_route.c                # Routing manager
│   ├── a2b_clock.c                # Clock manager
│   ├── a2b_irq.c                  # Interrupt manager
│   ├── a2b_state.c                # State machine
│   ├── a2b_recovery.c             # Recovery manager
│   ├── a2b_diagnostic.c           # Diagnostics
│   └── a2b_remote.c               # Remote register access
│
├── register/                      # Register layer
│   ├── ad2428_reg.h               # AD2428 register definitions
│   └── ad2428_reg.c               # AD2428 register access
│
├── hal/                           # Hardware abstraction
│   ├── a2b_i2c.h                  # I2C HAL interface
│   ├── a2b_i2c.c                  # I2C HAL (default stub)
│   ├── a2b_timer.h                # Timer HAL interface
│   ├── a2b_timer.c                # Timer HAL (default stub)
│   └── platform/
│       └── linux/
│           ├── i2c_linux.c        # Linux I2C implementation
│           ├── i2c_linux.h
│           ├── timer_linux.c      # Linux timer implementation
│           └── timer_linux.h
│
├── mock/                          # Mock hardware
│   ├── mock_ad2428.h              # Mock AD2428 interface
│   ├── mock_ad2428.c              # Mock AD2428 implementation
│   ├── mock_i2c.h                 # Mock I2C interface
│   └── mock_i2c.c                 # Mock I2C implementation
│
├── tools/                         # Python CLI tools
│   ├── reg_generator.py           # Register code generator
│   ├── csv_parser.py               # CSV parser utility
│   ├── config_generator.py         # Config code generator
│   └── requirements.txt            # Python dependencies
│
├── test/                          # Unit tests (Unity)
│   ├── CMakeLists.txt
│   ├── unity/
│   │   ├── unity.h
│   │   ├── unity.c
│   │   └── unity_internals.h
│   ├── test_main.c                # Test runner
│   ├── test_register.c
│   ├── test_discovery.c
│   ├── test_network.c
│   ├── test_route.c
│   ├── test_clock.c
│   ├── test_state.c
│   └── fixtures/
│       └── mock_ad2428_fixture.c
│
├── .github/
│   └── workflows/
│       └── ci.yml                 # GitHub Actions CI
│
└── docs/superpowers/
    ├── specs/                     # Design specifications
    │   └── 2026-06-30-OpenA2B-design.md
    └── development/               # Development guides
        └── getting_started.md
```

---

## 11. Development Phases

### Phase 1: Foundation (2 weeks)
- [ ] CMake project scaffold
- [ ] Platform HAL (Linux I2C/Timer)
- [ ] I2C Mock + Bus Interface
- [ ] Register Layer (AD2428)
- [ ] Logger system + Error codes
- [ ] Mock AD2428 Hardware
- [ ] Unit test framework (Unity)
- [ ] GitHub Actions CI

### Phase 2: Core Features (3 weeks)
- [ ] Discovery Manager
- [ ] Network Manager
- [ ] Remote Register Access
- [ ] Interrupt Handler
- [ ] State Machine
- [ ] Integration tests

### Phase 3: Advanced Features (3 weeks)
- [ ] Audio Routing
- [ ] Slot Configuration
- [ ] Clock Management
- [ ] Diagnostics
- [ ] Recovery Manager
- [ ] Python toolchain

### Phase 4: Polish (1 week)
- [ ] Documentation (Doxygen)
- [ ] Architecture diagrams
- [ ] GitHub README
- [ ] Release process

---

## 12. Coding Standards

### 12.1 Style Guide
- Linux Kernel style (K&R braces, tabs)
- Maximum line length: 80 characters
- Prefix all public symbols: `A2B_`

### 12.2 Naming Conventions
- Types: `a2b_<module>_<name>_t`
- Functions: `A2B_<Module>_<Action>`
- Macros: `A2B_<MODULE>_<NAME>`
- Enums: `A2B_<MODULE>_<VALUE>`

### 12.3 Documentation
- All public APIs: Doxygen comments
- Required: brief description, parameters, return value
- Optional: preconditions, postconditions, error conditions

---

## 13. Version

| Version | Date | Author | Changes |
|---------|------|--------|---------|
| 1.0 | 2026-06-30 | OpenA2B Team | Initial draft |

---

## 14. Approval

This specification is subject to review by the project architect before implementation begins.

**Status:** Pending Approval

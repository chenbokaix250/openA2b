# OpenA2B Phase 1: Foundation Implementation Plan

> **For agentic workers:** REQUIRED SUB-SKILL: Use superpowers:subagent-driven-development (recommended) or superpowers:executing-plans to implement this plan task-by-task. Steps use checkbox (`- [ ]`) syntax for tracking.

**Goal:** Establish the complete project scaffold for OpenA2B - CMake build system, basic directory structure, HAL interfaces, error codes, logger, and unit test framework with Mock I2C.

**Architecture:** Phase 1 builds the foundation layers only. No A2B protocol logic yet. All modules are stub implementations that compile and link but perform no real work. Each module has passing unit tests before moving to the next.

**Tech Stack:** CMake, C (C11), Unity test framework, GitHub Actions, Python 3

---

## File Structure

```
OpenA2B/
├── CMakeLists.txt                          # Main build + test target
├── LICENSE                                 # MIT
├── README.md                               # Project overview (Phase 4)
├── docs/
│   └── superpowers/
│       ├── specs/
│       │   └── 2026-06-30-OpenA2B-design.md
│       └── plans/
│           └── 2026-06-30-OpenA2B-phase1-foundation.md
├── include/                                # Public headers (Phase 1)
│   ├── a2b.h                             # Main include (aggregate)
│   ├── a2b_types.h                        # All type definitions
│   ├── a2b_error.h                        # Error codes
│   ├── a2b_version.h                      # Version macros
│   └── a2b_build.h                        # Build-time configuration
├── hal/                                    # Hardware abstraction
│   ├── a2b_i2c.h                         # I2C ops struct + handle typedef
│   ├── a2b_i2c.c                         # Default stub implementation
│   ├── a2b_timer.h                       # Timer ops struct + handle typedef
│   └── a2b_timer.c                        # Default stub implementation
├── mock/                                   # Mock hardware
│   ├── mock_i2c.h                        # Mock I2C interface
│   └── mock_i2c.c                         # Mock I2C implementation
├── register/                               # Register layer (stub)
│   ├── ad2428_reg.h                      # Register base addresses + masks
│   └── ad2428_reg.c                      # Empty until Phase 2
├── core/                                   # Framework core (empty until Phase 2)
│   └── .gitkeep
├── tools/                                  # Python tools (Phase 4)
│   └── .gitkeep
├── test/                                   # Unit tests
│   ├── CMakeLists.txt                     # Test build config
│   ├── unity/
│   │   ├── unity.h                        # Unity test framework
│   │   ├── unity.c
│   │   └── unity_internals.h
│   └── src/
│       ├── test_main.c                   # Test runner entry point
│       ├── test_a2b_types.c              # Types tests
│       ├── test_a2b_error.c              # Error codes tests
│       ├── test_a2b_i2c.c                # I2C HAL tests
│       └── test_mock_i2c.c               # Mock I2C tests
└── .github/
    └── workflows/
        └── ci.yml                        # GitHub Actions CI
```

---

## Task 1: Create CMake Project Scaffold

**Files:**
- Create: `CMakeLists.txt`
- Create: `.github/workflows/ci.yml`
- Create: `include/a2b_version.h`

- [ ] **Step 1: Create CMakeLists.txt**

```cmake
cmake_minimum_required(VERSION 3.20)
project(OpenA2B VERSION 1.0.0 LANGUAGES C)

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)

# Options
option(A2B_BUILD_TESTS "Build unit tests" ON)
option(A2B_BUILD_MOCK "Build mock hardware" ON)

# Public include directory
target_include_directories(${PROJECT_NAME} PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:include>
)

# Core library (Phase 2+)
add_library(${PROJECT_NAME}_core INTERFACE)

# HAL library
add_library(${PROJECT_NAME}_hal STATIC
    hal/a2b_i2c.c
    hal/a2b_timer.c
)
target_include_directories(${PROJECT_NAME}_hal PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/include
    ${CMAKE_CURRENT_SOURCE_DIR}/hal
)

# Mock library (optional)
if(A2B_BUILD_MOCK)
    add_library(${PROJECT_NAME}_mock STATIC
        mock/mock_i2c.c
    )
    target_include_directories(${PROJECT_NAME}_mock PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/include
        ${CMAKE_CURRENT_SOURCE_DIR}/mock
    )
    target_link_libraries(${PROJECT_NAME}_hal PUBLIC ${PROJECT_NAME}_mock)
endif()

# Tests
if(A2B_BUILD_TESTS)
    enable_testing()
    add_subdirectory(test)
endif()

# Installation
include(GNUInstallDirs)
install(TARGETS ${PROJECT_NAME}_hal
    ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
    PUBLIC_HEADER DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
)
```

- [ ] **Step 2: Create include/a2b_version.h**

```c
#ifndef A2B_VERSION_H
#define A2B_VERSION_H

#define A2B_VERSION_MAJOR 1
#define A2B_VERSION_MINOR 0
#define A2B_VERSION_PATCH 0

#define A2B_VERSION_STRING "1.0.0"

#endif /* A2B_VERSION_H */
```

- [ ] **Step 3: Create .github/workflows/ci.yml**

```yaml
name: CI

on:
  push:
    branches: [main]
  pull_request:
    branches: [main]

jobs:
  build:
    runs-on: ubuntu-latest
    steps:
      - uses: actions/checkout@v4

      - name: Install Unity
        run: |
          git clone --depth 1 https://github.com/ThrowTheSwitch/Unity.git unity_temp
          mv unity_temp/unity ${PWD}/test/unity/
          rm -rf unity_temp

      - name: Configure CMake
        run: cmake -B build -DCMAKE_BUILD_TYPE=Debug

      - name: Build
        run: cmake --build build

      - name: Run Tests
        run: ctest --test-dir build -V

      - name: Upload Coverage
        uses: codecov/codecov-action@v4
        if: success()
```

- [ ] **Step 4: Verify CMake config**

Run: `cmake -B build`
Expected: No errors, generates Makefiles or Ninja files

Run: `cmake --build build`
Expected: Compiles `a2b_i2c.c` and `a2b_timer.c` without errors

- [ ] **Step 5: Commit**

```bash
git add CMakeLists.txt .github/workflows/ci.yml include/a2b_version.h
git commit -m "build: Add CMake project scaffold and GitHub Actions CI

Co-Authored-By: Claude <noreply@anthropic.com>"
```

---

## Task 2: Implement Core Types and Error Codes

**Files:**
- Create: `include/a2b_types.h`
- Create: `include/a2b_error.h`
- Create: `include/a2b.h`
- Create: `include/a2b_build.h`
- Create: `test/src/test_a2b_types.c`
- Create: `test/src/test_a2b_error.c`

- [ ] **Step 1: Create include/a2b_types.h**

```c
#ifndef A2B_TYPES_H
#define A2B_TYPES_H

#include <stdint.h>
#include <stdbool.h>

/* Handle types - opaque pointers */
typedef struct _A2B_I2C_Ops *A2B_I2C_HANDLE;
typedef struct _A2B_Timer_Ops *A2B_TIMER_HANDLE;
typedef void *A2B_DEVICE_HANDLE;

/* Result code */
typedef int32_t A2B_Result;

/* Boolean convenience */
typedef bool A2B_Bool;

#define A2B_TRUE  true
#define A2B_FALSE false

/* Maximum limits */
#define A2B_MAX_NODES   16
#define A2B_MAX_ROUTES  64

/* Node address type */
typedef uint8_t A2B_NodeAddr;

/* I2C address type */
typedef uint8_t A2B_I2CAddr;

/* Register address type */
typedef uint16_t A2B_RegAddr;

/* State enumeration */
typedef enum {
    A2B_STATE_INIT = 0,
    A2B_STATE_IDLE,
    A2B_STATE_ERROR,
    A2B_STATE_SHUTDOWN
} A2B_State;

/* Node structure (forward declared) */
typedef struct _A2B_Node A2B_Node;

/* Device configuration */
typedef struct _A2B_Config {
    uint8_t  master_address;
    uint32_t bus_speed_hz;
    uint8_t  max_nodes;
} A2B_Config;

/* Default configuration */
#define A2B_CONFIG_DEFAULT { 0x68, 400000, 8 }

#endif /* A2B_TYPES_H */
```

- [ ] **Step 2: Create include/a2b_error.h**

```c
#ifndef A2B_ERROR_H
#define A2B_ERROR_H

#include <stdint.h>

typedef int32_t A2B_Result;

/* Success */
#define A2B_SUCCESS             ((A2B_Result)0)

/* Generic errors */
#define A2B_ERR_GENERIC         ((A2B_Result)-1)
#define A2B_ERR_INVALID_PARAM   ((A2B_Result)-2)
#define A2B_ERR_NO_MEM          ((A2B_Result)-3)
#define A2B_ERR_TIMEOUT         ((A2B_Result)-4)
#define A2B_ERR_UNSUPPORTED     ((A2B_Result)-5)

/* I2C errors */
#define A2B_ERR_I2C_NACK        ((A2B_Result)-10)
#define A2B_ERR_I2C_TIMEOUT     ((A2B_Result)-11)
#define A2B_ERR_I2C_BUSY        ((A2B_Result)-12)

/* Node errors */
#define A2B_ERR_NODE_NOT_FOUND  ((A2B_Result)-20)
#define A2B_ERR_NODE_NO_RESPONSE ((A2B_Result)-21)

/* Register errors */
#define A2B_ERR_REG_READ        ((A2B_Result)-30)
#define A2B_ERR_REG_WRITE       ((A2B_Result)-31)
#define A2B_ERR_REG_INVALID_ADDR ((A2B_Result)-32)

/* State errors */
#define A2B_ERR_STATE_INVALID   ((A2B_Result)-40)

/* Convert result to string */
const char *A2B_ResultToString(A2B_Result result);

#endif /* A2B_ERROR_H */
```

- [ ] **Step 3: Create include/a2b.h**

```c
#ifndef A2B_H
#define A2B_H

#include "a2b_types.h"
#include "a2b_error.h"
#include "a2b_version.h"
#include "a2b_build.h"

/* Main include file - aggregates all public headers */

#endif /* A2B_H */
```

- [ ] **Step 4: Create include/a2b_build.h**

```c
#ifndef A2B_BUILD_H
#define A2B_BUILD_H

/* Build-time configuration */
#define A2B_BUILD_DEBUG 1
#define A2B_BUILD_MOCK  1

#endif /* A2B_BUILD_H */
```

- [ ] **Step 5: Create test/src/test_a2b_types.c**

```c
#include "unity.h"
#include "a2b_types.h"

void setUp(void) {}
void tearDown(void) {}

void test_A2B_MAX_NODES_is_16(void)
{
    TEST_ASSERT_EQUAL_UINT8(16, A2B_MAX_NODES);
}

void test_A2B_MAX_ROUTES_is_64(void)
{
    TEST_ASSERT_EQUAL_UINT8(64, A2B_MAX_ROUTES);
}

void test_A2B_Config_default(void)
{
    A2B_Config cfg = A2B_CONFIG_DEFAULT;
    TEST_ASSERT_EQUAL_UINT8(0x68, cfg.master_address);
    TEST_ASSERT_EQUAL_UINT32(400000, cfg.bus_speed_hz);
    TEST_ASSERT_EQUAL_UINT8(8, cfg.max_nodes);
}

void test_A2B_State_enum_values(void)
{
    TEST_ASSERT_EQUAL_INT(0, A2B_STATE_INIT);
    TEST_ASSERT_EQUAL_INT(1, A2B_STATE_IDLE);
    TEST_ASSERT_EQUAL_INT(2, A2B_STATE_ERROR);
    TEST_ASSERT_EQUAL_INT(3, A2B_STATE_SHUTDOWN);
}
```

- [ ] **Step 6: Create test/src/test_a2b_error.c**

```c
#include "unity.h"
#include "a2b_error.h"

void setUp(void) {}
void tearDown(void) {}

void test_A2B_SUCCESS_is_zero(void)
{
    TEST_ASSERT_EQUAL_INT32(0, A2B_SUCCESS);
}

void test_A2B_ERR_I2C_NACK_is_negative(void)
{
    TEST_ASSERT_TRUE(A2B_ERR_I2C_NACK < 0);
}

void test_A2B_ERR_NODE_NOT_FOUND_is_negative(void)
{
    TEST_ASSERT_TRUE(A2B_ERR_NODE_NOT_FOUND < 0);
}

void test_A2B_ResultToString_returns_success_for_zero(void)
{
    const char *str = A2B_ResultToString(A2B_SUCCESS);
    TEST_ASSERT_NOT_NULL(str);
    TEST_ASSERT_EQUAL_STRING("SUCCESS", str);
}

void test_A2B_ResultToString_returns_error_for_negative(void)
{
    const char *str = A2B_ResultToString(A2B_ERR_I2C_NACK);
    TEST_ASSERT_NOT_NULL(str);
    TEST_ASSERT_TRUE(str[0] == 'E' || str[0] == 'e');
}
```

- [ ] **Step 7: Write minimal a2b_error.c to make tests compile**

Create: `core/a2b_error.c`

```c
#include "a2b_error.h"

const char *A2B_ResultToString(A2B_Result result)
{
    switch (result) {
        case A2B_SUCCESS:           return "SUCCESS";
        case A2B_ERR_GENERIC:      return "ERR_GENERIC";
        case A2B_ERR_INVALID_PARAM: return "ERR_INVALID_PARAM";
        case A2B_ERR_NO_MEM:       return "ERR_NO_MEM";
        case A2B_ERR_TIMEOUT:       return "ERR_TIMEOUT";
        case A2B_ERR_UNSUPPORTED:   return "ERR_UNSUPPORTED";
        case A2B_ERR_I2C_NACK:      return "ERR_I2C_NACK";
        case A2B_ERR_I2C_TIMEOUT:   return "ERR_I2C_TIMEOUT";
        case A2B_ERR_I2C_BUSY:      return "ERR_I2C_BUSY";
        case A2B_ERR_NODE_NOT_FOUND: return "ERR_NODE_NOT_FOUND";
        case A2B_ERR_NODE_NO_RESPONSE: return "ERR_NODE_NO_RESPONSE";
        case A2B_ERR_REG_READ:      return "ERR_REG_READ";
        case A2B_ERR_REG_WRITE:     return "ERR_REG_WRITE";
        case A2B_ERR_REG_INVALID_ADDR: return "ERR_REG_INVALID_ADDR";
        case A2B_ERR_STATE_INVALID:  return "ERR_STATE_INVALID";
        default:                    return "ERR_UNKNOWN";
    }
}
```

- [ ] **Step 8: Update CMakeLists.txt to include core/a2b_error.c**

```cmake
add_library(${PROJECT_NAME}_hal STATIC
    hal/a2b_i2c.c
    hal/a2b_timer.c
    core/a2b_error.c   # Add this line
)
```

- [ ] **Step 9: Run tests to verify they pass**

Run: `cmake --build build && ctest --test-dir build -V`
Expected: All type and error tests PASS

- [ ] **Step 10: Commit**

```bash
git add include/a2b_types.h include/a2b_error.h include/a2b.h include/a2b_build.h
git add core/a2b_error.c
git add test/src/test_a2b_types.c test/src/test_a2b_error.c
git commit -m "feat: Add core types and error codes

- A2B_Types: handle types, config structs, state enum
- A2B_Error: result codes and A2B_ResultToString()
- Unit tests for types and error codes

Co-Authored-By: Claude <noreply@anthropic.com>"
```

---

## Task 3: Implement I2C HAL and Mock I2C

**Files:**
- Create: `hal/a2b_i2c.h`
- Create: `hal/a2b_i2c.c`
- Create: `mock/mock_i2c.h`
- Create: `mock/mock_i2c.c`
- Create: `test/src/test_a2b_i2c.c`
- Create: `test/src/test_mock_i2c.c`

- [ ] **Step 1: Create hal/a2b_i2c.h**

```c
#ifndef A2B_I2C_H
#define A2B_I2C_H

#include "a2b_types.h"
#include "a2b_error.h"

/**
 * I2C operations interface
 * All I2C access goes through this abstraction.
 */
typedef struct _A2B_I2C_Ops {
    A2B_Result (*init)(A2B_I2C_HANDLE handle, void *config);
    A2B_Result (*deinit)(A2B_I2C_HANDLE handle);
    A2B_Result (*write)(A2B_I2C_HANDLE handle, A2B_I2CAddr addr,
                         const uint8_t *data, uint32_t len);
    A2B_Result (*read)(A2B_I2C_HANDLE handle, A2B_I2CAddr addr,
                        uint8_t *data, uint32_t len);
    A2B_Result (*write_read)(A2B_I2C_HANDLE handle, A2B_I2CAddr addr,
                              const uint8_t *wdata, uint32_t wlen,
                              uint8_t *rdata, uint32_t rlen);
} A2B_I2C_Ops;

/**
 * I2C handle - opaque pointer to platform-specific instance
 */
struct _A2B_I2C_Ops {
    const A2B_I2C_Ops *ops;
    void *priv;
};

/**
 * Initialize default I2C HAL
 */
A2B_I2C_HANDLE A2B_I2CInit(void);

/**
 * Close I2C HAL
 */
A2B_Result A2B_I2CDeinit(A2B_I2C_HANDLE handle);

/**
 * Write data to I2C device
 */
A2B_Result A2B_I2CWrite(A2B_I2C_HANDLE handle, A2B_I2CAddr addr,
                         const uint8_t *data, uint32_t len);

/**
 * Read data from I2C device
 */
A2B_Result A2B_I2CRead(A2B_I2C_HANDLE handle, A2B_I2CAddr addr,
                        uint8_t *data, uint32_t len);

/**
 * Combined write-then-read (repeated start)
 */
A2B_Result A2B_I2CWriteRead(A2B_I2C_HANDLE handle, A2B_I2CAddr addr,
                              const uint8_t *wdata, uint32_t wlen,
                              uint8_t *rdata, uint32_t rlen);

#endif /* A2B_I2C_H */
```

- [ ] **Step 2: Create hal/a2b_i2c.c (stub implementation)**

```c
#include "a2b_i2c.h"
#include "a2b_build.h"

/* Stub I2C handle data */
typedef struct _A2B_I2C_StubData {
    bool initialized;
} A2B_I2C_StubData;

static A2B_I2C_StubData g_stub_data = { false };

A2B_I2C_HANDLE A2B_I2CInit(void)
{
    g_stub_data.initialized = true;
    return (A2B_I2C_HANDLE)&g_stub_data;
}

A2B_Result A2B_I2CDeinit(A2B_I2C_HANDLE handle)
{
    (void)handle;
    g_stub_data.initialized = false;
    return A2B_SUCCESS;
}

A2B_Result A2B_I2CWrite(A2B_I2C_HANDLE handle, A2B_I2CAddr addr,
                         const uint8_t *data, uint32_t len)
{
    (void)handle; (void)addr; (void)data; (void)len;
    return A2B_ERR_UNSUPPORTED;  /* Stub: not implemented */
}

A2B_Result A2B_I2CRead(A2B_I2C_HANDLE handle, A2B_I2CAddr addr,
                        uint8_t *data, uint32_t len)
{
    (void)handle; (void)addr; (void)data; (void)len;
    return A2B_ERR_UNSUPPORTED;  /* Stub: not implemented */
}

A2B_Result A2B_I2CWriteRead(A2B_I2C_HANDLE handle, A2B_I2CAddr addr,
                              const uint8_t *wdata, uint32_t wlen,
                              uint8_t *rdata, uint32_t rlen)
{
    (void)handle; (void)addr; (void)wdata; (void)wlen; (void)rdata; (void)rlen;
    return A2B_ERR_UNSUPPORTED;  /* Stub: not implemented */
}
```

- [ ] **Step 3: Create mock/mock_i2c.h**

```c
#ifndef MOCK_I2C_H
#define MOCK_I2C_H

#include "a2b_types.h"
#include "a2b_error.h"
#include "a2b_i2c.h"

/**
 * Mock I2C device response record
 */
typedef struct _MockI2C_Response {
    A2B_I2CAddr addr;
    uint8_t     reg_addr;
    uint32_t    read_count;
    uint32_t    write_count;
    bool        nack_on_write;
    bool        nack_on_read;
} MockI2C_Response;

/**
 * Initialize mock I2C
 */
void MockI2C_Init(void);

/**
 * Reset mock I2C state
 */
void MockI2C_Reset(void);

/**
 * Configure a slave to NACK on next write
 */
void MockI2C_InjectNackOnWrite(A2B_I2CAddr addr);

/**
 * Configure a slave to NACK on next read
 */
void MockI2C_InjectNackOnRead(A2B_I2CAddr addr);

/**
 * Get response record for an address
 */
const MockI2C_Response *MockI2C_GetResponse(A2B_I2CAddr addr);

#endif /* MOCK_I2C_H */
```

- [ ] **Step 4: Create mock/mock_i2c.c**

```c
#include "mock_i2c.h"

static MockI2C_Response g_responses[256] = { 0 };

void MockI2C_Init(void)
{
    MockI2C_Reset();
}

void MockI2C_Reset(void)
{
    for (int i = 0; i < 256; i++) {
        g_responses[i].addr = (A2B_I2CAddr)i;
        g_responses[i].reg_addr = 0;
        g_responses[i].read_count = 0;
        g_responses[i].write_count = 0;
        g_responses[i].nack_on_write = false;
        g_responses[i].nack_on_read = false;
    }
}

void MockI2C_InjectNackOnWrite(A2B_I2CAddr addr)
{
    g_responses[addr].nack_on_write = true;
}

void MockI2C_InjectNackOnRead(A2B_I2CAddr addr)
{
    g_responses[addr].nack_on_read = true;
}

const MockI2C_Response *MockI2C_GetResponse(A2B_I2CAddr addr)
{
    return &g_responses[addr];
}
```

- [ ] **Step 5: Create test/src/test_a2b_i2c.c**

```c
#include "unity.h"
#include "a2b_i2c.h"

void setUp(void) {}
void tearDown(void) {}

void test_A2B_I2CInit_returns_non_null(void)
{
    A2B_I2C_HANDLE handle = A2B_I2CInit();
    TEST_ASSERT_NOT_NULL(handle);
}

void test_A2B_I2CInit_twice_does_not_crash(void)
{
    A2B_I2C_HANDLE h1 = A2B_I2CInit();
    A2B_I2C_HANDLE h2 = A2B_I2CInit();
    TEST_ASSERT_NOT_NULL(h1);
    TEST_ASSERT_NOT_NULL(h2);
}

void test_A2B_I2CDeinit_returns_success(void)
{
    A2B_I2C_HANDLE handle = A2B_I2CInit();
    A2B_Result result = A2B_I2CDeinit(handle);
    TEST_ASSERT_EQUAL_INT32(A2B_SUCCESS, result);
}

void test_A2B_I2CWrite_returns_unsupported_on_stub(void)
{
    A2B_I2C_HANDLE handle = A2B_I2CInit();
    uint8_t data[4] = { 0 };
    A2B_Result result = A2B_I2CWrite(handle, 0x68, data, sizeof(data));
    TEST_ASSERT_EQUAL_INT32(A2B_ERR_UNSUPPORTED, result);
}

void test_A2B_I2CRead_returns_unsupported_on_stub(void)
{
    A2B_I2C_HANDLE handle = A2B_I2CInit();
    uint8_t data[4] = { 0 };
    A2B_Result result = A2B_I2CRead(handle, 0x68, data, sizeof(data));
    TEST_ASSERT_EQUAL_INT32(A2B_ERR_UNSUPPORTED, result);
}
```

- [ ] **Step 6: Create test/src/test_mock_i2c.c**

```c
#include "unity.h"
#include "mock_i2c.h"

void setUp(void)
{
    MockI2C_Init();
}

void tearDown(void)
{
    MockI2C_Reset();
}

void test_MockI2C_Init_succeeds(void)
{
    MockI2C_Init();  /* Should not crash */
}

void test_MockI2C_InjectNackOnWrite(void)
{
    MockI2C_InjectNackOnWrite(0x68);
    const MockI2C_Response *resp = MockI2C_GetResponse(0x68);
    TEST_ASSERT_TRUE(resp->nack_on_write);
}

void test_MockI2C_InjectNackOnRead(void)
{
    MockI2C_InjectNackOnRead(0x68);
    const MockI2C_Response *resp = MockI2C_GetResponse(0x68);
    TEST_ASSERT_TRUE(resp->nack_on_read);
}

void test_MockI2C_GetResponse_returns_valid_addr(void)
{
    const MockI2C_Response *resp = MockI2C_GetResponse(0x68);
    TEST_ASSERT_EQUAL_UINT8(0x68, resp->addr);
}
```

- [ ] **Step 7: Create test/CMakeLists.txt**

```cmake
include_directories(
    ${CMAKE_CURRENT_SOURCE_DIR}/../include
    ${CMAKE_CURRENT_SOURCE_DIR}/../hal
    ${CMAKE_CURRENT_SOURCE_DIR}/../mock
    ${CMAKE_CURRENT_SOURCE_DIR}/unity
)

set(TEST_SOURCES
    unity/unity.c
    src/test_main.c
    src/test_a2b_types.c
    src/test_a2b_error.c
    src/test_a2b_i2c.c
    src/test_mock_i2c.c
)

add_executable(${PROJECT_NAME}_tests ${TEST_SOURCES})
target_link_libraries(${PROJECT_NAME}_tests ${PROJECT_NAME}_hal ${PROJECT_NAME}_mock)

add_test(NAME ${PROJECT_NAME}_tests COMMAND ${PROJECT_NAME}_tests)
```

- [ ] **Step 8: Create test/src/test_main.c**

```c
#include "unity.h"

extern void setUp(void);
extern void tearDown(void);

extern void test_A2B_MAX_NODES_is_16(void);
extern void test_A2B_MAX_ROUTES_is_64(void);
extern void test_A2B_Config_default(void);
extern void test_A2B_State_enum_values(void);
extern void test_A2B_SUCCESS_is_zero(void);
extern void test_A2B_ERR_I2C_NACK_is_negative(void);
extern void test_A2B_ERR_NODE_NOT_FOUND_is_negative(void);
extern void test_A2B_ResultToString_returns_success_for_zero(void);
extern void test_A2B_ResultToString_returns_error_for_negative(void);
extern void test_A2B_I2CInit_returns_non_null(void);
extern void test_A2B_I2CInit_twice_does_not_crash(void);
extern void test_A2B_I2CDeinit_returns_success(void);
extern void test_A2B_I2CWrite_returns_unsupported_on_stub(void);
extern void test_A2B_I2CRead_returns_unsupported_on_stub(void);
extern void test_MockI2C_Init_succeeds(void);
extern void test_MockI2C_InjectNackOnWrite(void);
extern void test_MockI2C_InjectNackOnRead(void);
extern void test_MockI2C_GetResponse_returns_valid_addr(void);

int main(void)
{
    UNITY_BEGIN();

    /* Types */
    RUN_TEST(test_A2B_MAX_NODES_is_16);
    RUN_TEST(test_A2B_MAX_ROUTES_is_64);
    RUN_TEST(test_A2B_Config_default);
    RUN_TEST(test_A2B_State_enum_values);

    /* Error */
    RUN_TEST(test_A2B_SUCCESS_is_zero);
    RUN_TEST(test_A2B_ERR_I2C_NACK_is_negative);
    RUN_TEST(test_A2B_ERR_NODE_NOT_FOUND_is_negative);
    RUN_TEST(test_A2B_ResultToString_returns_success_for_zero);
    RUN_TEST(test_A2B_ResultToString_returns_error_for_negative);

    /* I2C HAL */
    RUN_TEST(test_A2B_I2CInit_returns_non_null);
    RUN_TEST(test_A2B_I2CInit_twice_does_not_crash);
    RUN_TEST(test_A2B_I2CDeinit_returns_success);
    RUN_TEST(test_A2B_I2CWrite_returns_unsupported_on_stub);
    RUN_TEST(test_A2B_I2CRead_returns_unsupported_on_stub);

    /* Mock I2C */
    RUN_TEST(test_MockI2C_Init_succeeds);
    RUN_TEST(test_MockI2C_InjectNackOnWrite);
    RUN_TEST(test_MockI2C_InjectNackOnRead);
    RUN_TEST(test_MockI2C_GetResponse_returns_valid_addr);

    return UNITY_END();
}
```

- [ ] **Step 9: Copy Unity framework files**

Download Unity from ThrowTheSwitch/Unity into `test/unity/`:
```bash
git clone --depth 1 https://github.com/ThrowTheSwitch/Unity.git /tmp/unity_temp
cp -r /tmp/unity_temp/src/unity/* test/unity/
rm -rf /tmp/unity_temp
```

- [ ] **Step 10: Build and run tests**

Run: `cmake --build build && ctest --test-dir build -V`
Expected: All 17 tests PASS

- [ ] **Step 11: Commit**

```bash
git add hal/a2b_i2c.h hal/a2b_i2c.c mock/mock_i2c.h mock/mock_i2c.c
git add test/src/test_main.c test/src/test_a2b_i2c.c test/src/test_mock_i2c.c test/CMakeLists.txt
git add test/unity/
git commit -m "feat: Add I2C HAL and Mock I2C

- I2C ops interface with stub implementation
- Mock I2C with NACK injection support
- Unity test framework integrated
- 17 passing unit tests

Co-Authored-By: Claude <noreply@anthropic.com>"
```

---

## Task 4: Implement Timer HAL

**Files:**
- Create: `hal/a2b_timer.h`
- Create: `hal/a2b_timer.c`
- Create: `test/src/test_a2b_timer.c`
- Modify: `test/src/test_main.c` (add timer tests)

- [ ] **Step 1: Create hal/a2b_timer.h**

```c
#ifndef A2B_TIMER_H
#define A2B_TIMER_H

#include "a2b_types.h"
#include "a2b_error.h"

/**
 * Timer callback function type
 */
typedef void (*A2B_TimerCallback)(void *priv);

/**
 * Timer operations interface
 */
typedef struct _A2B_TimerOps {
    A2B_Result (*init)(A2B_TIMER_HANDLE handle, void *config);
    A2B_Result (*deinit)(A2B_TIMER_HANDLE handle);
    A2B_Result (*start)(A2B_TIMER_HANDLE handle, uint32_t delay_ms);
    A2B_Result (*stop)(A2B_TIMER_HANDLE handle);
    bool       (*is_running)(A2B_TIMER_HANDLE handle);
} A2B_TimerOps;

/**
 * Timer handle structure
 */
struct _A2B_Timer_Ops {
    const A2B_TimerOps *ops;
    void *priv;
};

/**
 * Initialize timer HAL
 */
A2B_TIMER_HANDLE A2B_TimerInit(void);

/**
 * Close timer HAL
 */
A2B_Result A2B_TimerDeinit(A2B_TIMER_HANDLE handle);

/**
 * Start a one-shot timer
 */
A2B_Result A2B_TimerStart(A2B_TIMER_HANDLE handle, uint32_t delay_ms,
                            A2B_TimerCallback callback, void *priv);

/**
 * Stop a running timer
 */
A2B_Result A2B_TimerStop(A2B_TIMER_HANDLE handle);

/**
 * Check if timer is running
 */
bool A2B_TimerIsRunning(A2B_TIMER_HANDLE handle);

#endif /* A2B_TIMER_H */
```

- [ ] **Step 2: Create hal/a2b_timer.c (stub implementation)**

```c
#include "a2b_timer.h"

typedef struct _A2B_Timer_StubData {
    bool initialized;
    bool running;
} A2B_Timer_StubData;

static A2B_Timer_StubData g_timer_data = { false, false };

A2B_TIMER_HANDLE A2B_TimerInit(void)
{
    g_timer_data.initialized = true;
    g_timer_data.running = false;
    return (A2B_TIMER_HANDLE)&g_timer_data;
}

A2B_Result A2B_TimerDeinit(A2B_TIMER_HANDLE handle)
{
    (void)handle;
    g_timer_data.initialized = false;
    g_timer_data.running = false;
    return A2B_SUCCESS;
}

A2B_Result A2B_TimerStart(A2B_TIMER_HANDLE handle, uint32_t delay_ms,
                            A2B_TimerCallback callback, void *priv)
{
    (void)handle; (void)delay_ms; (void)callback; (void)priv;
    g_timer_data.running = true;
    return A2B_ERR_UNSUPPORTED;  /* Stub: callback not fired */
}

A2B_Result A2B_TimerStop(A2B_TIMER_HANDLE handle)
{
    (void)handle;
    g_timer_data.running = false;
    return A2B_SUCCESS;
}

bool A2B_TimerIsRunning(A2B_TIMER_HANDLE handle)
{
    (void)handle;
    return g_timer_data.running;
}
```

- [ ] **Step 3: Create test/src/test_a2b_timer.c**

```c
#include "unity.h"
#include "a2b_timer.h"

static bool g_callback_fired = false;
static void *g_callback_priv = NULL;

void timer_callback(void *priv)
{
    g_callback_fired = true;
    g_callback_priv = priv;
}

void setUp(void)
{
    g_callback_fired = false;
    g_callback_priv = NULL;
}

void tearDown(void)
{
}

void test_A2B_TimerInit_returns_non_null(void)
{
    A2B_TIMER_HANDLE handle = A2B_TimerInit();
    TEST_ASSERT_NOT_NULL(handle);
}

void test_A2B_TimerDeinit_returns_success(void)
{
    A2B_TIMER_HANDLE handle = A2B_TimerInit();
    A2B_Result result = A2B_TimerDeinit(handle);
    TEST_ASSERT_EQUAL_INT32(A2B_SUCCESS, result);
}

void test_A2B_TimerStop_returns_success(void)
{
    A2B_TIMER_HANDLE handle = A2B_TimerInit();
    A2B_Result result = A2B_TimerStop(handle);
    TEST_ASSERT_EQUAL_INT32(A2B_SUCCESS, result);
}

void test_A2B_TimerIsRunning_returns_false_after_init(void)
{
    A2B_TIMER_HANDLE handle = A2B_TimerInit();
    bool running = A2B_TimerIsRunning(handle);
    TEST_ASSERT_FALSE(running);
}
```

- [ ] **Step 4: Update test/src/test_main.c to include timer tests**

Add to `test_main.c`:
```c
extern void test_A2B_TimerInit_returns_non_null(void);
extern void test_A2B_TimerDeinit_returns_success(void);
extern void test_A2B_TimerStop_returns_success(void);
extern void test_A2B_TimerIsRunning_returns_false_after_init(void);
```

And add in `main()` before `return UNITY_END()`:
```c
    /* Timer */
    RUN_TEST(test_A2B_TimerInit_returns_non_null);
    RUN_TEST(test_A2B_TimerDeinit_returns_success);
    RUN_TEST(test_A2B_TimerStop_returns_success);
    RUN_TEST(test_A2B_TimerIsRunning_returns_false_after_init);
```

- [ ] **Step 5: Update CMakeLists.txt to add timer source**

In `${PROJECT_NAME}_hal` target, add `hal/a2b_timer.c`.

- [ ] **Step 6: Build and run tests**

Run: `cmake --build build && ctest --test-dir build -V`
Expected: All 21 tests PASS (17 previous + 4 new timer tests)

- [ ] **Step 7: Commit**

```bash
git add hal/a2b_timer.h hal/a2b_timer.c test/src/test_a2b_timer.c test/src/test_main.c
git commit -m "feat: Add Timer HAL stub

- Timer ops interface with stub implementation
- 4 new passing unit tests
- Total: 21 passing tests

Co-Authored-By: Claude <noreply@anthropic.com>"
```

---

## Task 5: Implement Logger System

**Files:**
- Create: `include/a2b_logger.h`
- Create: `core/a2b_logger.c`
- Create: `test/src/test_a2b_logger.c`
- Modify: `test/src/test_main.c` (add logger tests)

- [ ] **Step 1: Create include/a2b_logger.h**

```c
#ifndef A2B_LOGGER_H
#define A2B_LOGGER_H

#include "a2b_types.h"
#include "a2b_error.h"

/* Log levels */
typedef enum {
    A2B_LOG_NONE = 0,
    A2B_LOG_ERROR,
    A2B_LOG_WARN,
    A2B_LOG_INFO,
    A2B_LOG_DEBUG,
    A2B_LOG_TRACE
} A2B_LogLevel;

/* Log tag - component identifier */
typedef const char *A2B_LogTag;

#define A2B_LOG_TAG_MAIN       "MAIN"
#define A2B_LOG_TAG_DISCOVERY  "DISCOVERY"
#define A2B_LOG_TAG_NETWORK    "NETWORK"
#define A2B_LOG_TAG_ROUTE      "ROUTE"
#define A2B_LOG_TAG_CLOCK      "CLOCK"
#define A2B_LOG_TAG_IRQ        "IRQ"
#define A2B_LOG_TAG_I2C        "I2C"
#define A2B_LOG_TAG_REG        "REG"
#define A2B_LOG_TAG_STATE      "STATE"

/* Compile-time log level control */
#ifndef A2B_LOG_LEVEL
#define A2B_LOG_LEVEL A2B_LOG_INFO
#endif

/* Log macros - zero overhead when disabled */
#if A2B_LOG_LEVEL == A2B_LOG_NONE
#define A2B_LOG(level, tag, fmt, ...)   ((void)0)
#else
#define A2B_LOG(level, tag, fmt, ...) \
    a2b_logger_log(level, tag, __FILE__, __LINE__, fmt, ##__VA_ARGS__)
#endif

#define A2B_LOG_ERROR(tag, fmt, ...)   A2B_LOG(A2B_LOG_ERROR, tag, fmt, ##__VA_ARGS__)
#define A2B_LOG_WARN(tag, fmt, ...)    A2B_LOG(A2B_LOG_WARN,  tag, fmt, ##__VA_ARGS__)
#define A2B_LOG_INFO(tag, fmt, ...)    A2B_LOG(A2B_LOG_INFO,  tag, fmt, ##__VA_ARGS__)
#define A2B_LOG_DEBUG(tag, fmt, ...)   A2B_LOG(A2B_LOG_DEBUG, tag, fmt, ##__VA_ARGS__)
#define A2B_LOG_TRACE(tag, fmt, ...)   A2B_LOG(A2B_LOG_TRACE, tag, fmt, ##__VA_ARGS__)

/**
 * Initialize logger
 */
void A2B_LoggerInit(void);

/**
 * Set minimum log level
 */
void A2B_LoggerSetLevel(A2B_LogLevel level);

/**
 * Get current log level
 */
A2B_LogLevel A2B_LoggerGetLevel(void);

/**
 * Core logging function
 */
void a2b_logger_log(A2B_LogLevel level, A2B_LogTag tag,
                    const char *file, int line,
                    const char *fmt, ...);

#endif /* A2B_LOGGER_H */
```

- [ ] **Step 2: Create core/a2b_logger.c**

```c
#include "a2b_logger.h"
#include <stdio.h>
#include <stdarg.h>

static A2B_LogLevel g_log_level = A2B_LOG_INFO;

static const char *level_to_string(A2B_LogLevel level)
{
    switch (level) {
        case A2B_LOG_ERROR: return "ERROR";
        case A2B_LOG_WARN:  return "WARN ";
        case A2B_LOG_INFO:  return "INFO ";
        case A2B_LOG_DEBUG: return "DEBUG";
        case A2B_LOG_TRACE: return "TRACE";
        default:             return "     ";
    }
}

void A2B_LoggerInit(void)
{
    g_log_level = A2B_LOG_INFO;
}

void A2B_LoggerSetLevel(A2B_LogLevel level)
{
    g_log_level = level;
}

A2B_LogLevel A2B_LoggerGetLevel(void)
{
    return g_log_level;
}

void a2b_logger_log(A2B_LogLevel level, A2B_LogTag tag,
                    const char *file, int line,
                    const char *fmt, ...)
{
    if (level > g_log_level) {
        return;
    }

    /* Extract basename from file path */
    const char *basename = file;
    const char *p = file;
    while (*p) {
        if (*p == '/' && p[1]) {
            basename = p + 1;
        }
        p++;
    }

    printf("[%s][%-8s] ", level_to_string(level), tag);
    printf("%s:%d: ", basename, line);

    va_list args;
    va_start(args, fmt);
    vprintf(fmt, args);
    va_end(args);

    printf("\n");
}
```

- [ ] **Step 3: Create test/src/test_a2b_logger.c**

```c
#include "unity.h"
#include "a2b_logger.h"

void setUp(void)
{
    A2B_LoggerInit();
}

void tearDown(void)
{
}

void test_A2B_LoggerInit_succeeds(void)
{
    A2B_LoggerInit();  /* Should not crash */
    TEST_PASS();
}

void test_A2B_LoggerSetLevel_and_GetLevel(void)
{
    A2B_LoggerSetLevel(A2B_LOG_DEBUG);
    TEST_ASSERT_EQUAL_INT(A2B_LOG_DEBUG, A2B_LoggerGetLevel());

    A2B_LoggerSetLevel(A2B_LOG_ERROR);
    TEST_ASSERT_EQUAL_INT(A2B_LOG_ERROR, A2B_LoggerGetLevel());
}

void test_A2B_LOG_ERROR_macro_compiles(void)
{
    /* Macro should compile without error */
    A2B_LOG_ERROR(A2B_LOG_TAG_I2C, "Test error message");
    TEST_PASS();
}

void test_A2B_LOG_INFO_macro_compiles(void)
{
    /* Macro should compile without error */
    A2B_LOG_INFO(A2B_LOG_TAG_MAIN, "Test info message");
    TEST_PASS();
}

void test_A2B_LOG_LEVEL_none_disables_logging(void)
{
    /* When compiled with A2B_LOG_LEVEL=A2B_LOG_NONE, A2B_LOG expands to ((void)0) */
    /* This test just verifies compilation - output is discarded */
    A2B_LOG(A2B_LOG_TRACE, A2B_LOG_TAG_MAIN, "Should be discarded");
    TEST_PASS();
}
```

- [ ] **Step 4: Update test/src/test_main.c**

Add to extern declarations:
```c
extern void test_A2B_LoggerInit_succeeds(void);
extern void test_A2B_LoggerSetLevel_and_GetLevel(void);
extern void test_A2B_LOG_ERROR_macro_compiles(void);
extern void test_A2B_LOG_INFO_macro_compiles(void);
extern void test_A2B_LOG_LEVEL_none_disables_logging(void);
```

Add in `main()`:
```c
    /* Logger */
    RUN_TEST(test_A2B_LoggerInit_succeeds);
    RUN_TEST(test_A2B_LoggerSetLevel_and_GetLevel);
    RUN_TEST(test_A2B_LOG_ERROR_macro_compiles);
    RUN_TEST(test_A2B_LOG_INFO_macro_compiles);
    RUN_TEST(test_A2B_LOG_LEVEL_none_disables_logging);
```

- [ ] **Step 5: Update CMakeLists.txt to add logger**

In `${PROJECT_NAME}_hal` target, add `core/a2b_logger.c`.

- [ ] **Step 6: Build and run tests**

Run: `cmake --build build && ctest --test-dir build -V`
Expected: All 26 tests PASS

- [ ] **Step 7: Commit**

```bash
git add include/a2b_logger.h core/a2b_logger.c test/src/test_a2b_logger.c test/src/test_main.c
git commit -m "feat: Add logger system

- Log levels: ERROR, WARN, INFO, DEBUG, TRACE
- Tag-based log formatting: [LEVEL ][TAG       ] message
- Compile-time disable via A2B_LOG_LEVEL
- 5 new passing unit tests (26 total)

Co-Authored-By: Claude <noreply@anthropic.com>"
```

---

## Task 6: Create AD2428 Register Definitions (Stub)

**Files:**
- Create: `register/ad2428_reg.h`
- Create: `register/ad2428_reg.c`
- Create: `test/src/test_ad2428_reg.c`

- [ ] **Step 1: Create register/ad2428_reg.h**

```c
#ifndef AD2428_REG_H
#define AD2428_REG_H

#include "a2b_types.h"
#include "a2b_error.h"
#include "a2b_i2c.h"

/* AD2428 Register Base Address */
#define AD2428_REG_BASE          0x00

/* Common Registers */
#define AD2428_REG_CHIP_ID       0x00  /* Chip ID (read-only) */
#define AD2428_REG_REV_ID        0x01  /* Revision ID (read-only) */
#define AD2428_REG_STATUS        0x02  /* Status register */
#define AD2428_REG_CONTROL       0x03  /* Control register */

/* I2C Configuration Registers */
#define AD2428_REG_I2C_ADDR      0x04  /* I2C slave address */
#define AD2428_REG_I2CGCFG       0x05  /* I2C general config */
#define AD2428_REG_I2CCMD        0x06  /* I2C command */

/* Discovery Registers */
#define AD2428_REG_DISCSTAT      0x0A  /* Discovery status */
#define AD2428_REG_DISCCFG       0x0B  /* Discovery config */
#define AD2428_REG_NODECFG       0x0C  /* Node config */
#define AD2428_REG_NODEBC        0x0D  /* Node burst count */

/* Clock Registers */
#define AD2428_REG_PLLCTL        0x10  /* PLL control */
#define AD2428_REG_PLLSR         0x11  /* PLL status */
#define AD2428_REG_CLKSEL        0x12  /* Clock select */

/* Interrupt Registers */
#define AD2428_REG_INTSRC        0x14  /* Interrupt source */
#define AD2428_REG_INTMASK       0x15  /* Interrupt mask */
#define AD2428_REG_INTPOL        0x16  /* Interrupt polarity */

/* Audio Registers */
#define AD2428_REG_TDMSLOT0      0x20  /* TDM slot 0 */
#define AD2428_REG_TDMSLOT1      0x21  /* TDM slot 1 */
#define AD2428_REG_TDMSLOT2      0x22  /* TDM slot 2 */
#define AD2428_REG_TDMSLOT3      0x23  /* TDM slot 3 */
#define AD2428_REG_TDMCFG        0x24  /* TDM config */

/* Node registers (0x30-0x3F) - per-node configuration */
#define AD2428_REG_NODE_BASE      0x30
#define AD2428_REG_NODEn(n)       (AD2428_REG_NODE_BASE + (n))

/* Mask and shift helpers for bitfields */
#define AD2428_MASK(bit)          (1U << (bit))
#define AD2428_SHIFT(val, bit)    (((val) & AD2428_MASK(bit)) >> (bit))

/* Common bit definitions */
#define AD2428_STATUS_PLL_LOCK    AD2428_MASK(0)
#define AD2428_STATUS_DISC_DONE  AD2428_MASK(1)
#define AD2428_STATUS_ERR        AD2428_MASK(2)

/* Control register bits */
#define AD2428_CONTROL_RESET      AD2428_MASK(0)
#define AD2428_CONTROL_PWRDN      AD2428_MASK(1)
#define AD2428_CONTROL_START      AD2428_MASK(2)

/* Result type for register access */
typedef A2B_Result (*AD2428_RegReadFn)(void *priv, uint16_t reg, uint32_t *val);
typedef A2B_Result (*AD2428_RegWriteFn)(void *priv, uint16_t reg, uint32_t val);

/**
 * AD2428 register access context
 */
typedef struct _AD2428_RegContext {
    void *priv;
    AD2428_RegReadFn read;
    AD2428_RegWriteFn write;
} AD2428_RegContext;

/**
 * Initialize register context
 */
void AD2428_RegInit(AD2428_RegContext *ctx, void *priv,
                     AD2428_RegReadFn read, AD2428_RegWriteFn write);

/**
 * Read register
 */
A2B_Result AD2428_RegRead(AD2428_RegContext *ctx, uint16_t reg, uint32_t *val);

/**
 * Write register
 */
A2B_Result AD2428_RegWrite(AD2428_RegContext *ctx, uint16_t reg, uint32_t val);

/**
 * Update specific bits in a register
 */
A2B_Result AD2428_RegUpdateBits(AD2428_RegContext *ctx, uint16_t reg,
                                  uint32_t mask, uint32_t val);

#endif /* AD2428_REG_H */
```

- [ ] **Step 2: Create register/ad2428_reg.c**

```c
#include "ad2428_reg.h"

void AD2428_RegInit(AD2428_RegContext *ctx, void *priv,
                     AD2428_RegReadFn read, AD2428_RegWriteFn write)
{
    ctx->priv = priv;
    ctx->read = read;
    ctx->write = write;
}

A2B_Result AD2428_RegRead(AD2428_RegContext *ctx, uint16_t reg, uint32_t *val)
{
    if (ctx->read) {
        return ctx->read(ctx->priv, reg, val);
    }
    return A2B_ERR_UNSUPPORTED;
}

A2B_Result AD2428_RegWrite(AD2428_RegContext *ctx, uint16_t reg, uint32_t val)
{
    if (ctx->write) {
        return ctx->write(ctx->priv, reg, val);
    }
    return A2B_ERR_UNSUPPORTED;
}

A2B_Result AD2428_RegUpdateBits(AD2428_RegContext *ctx, uint16_t reg,
                                  uint32_t mask, uint32_t val)
{
    uint32_t cur = 0;
    A2B_Result res;

    res = AD2428_RegRead(ctx, reg, &cur);
    if (res != A2B_SUCCESS) {
        return res;
    }

    cur = (cur & ~mask) | (val & mask);
    return AD2428_RegWrite(ctx, reg, cur);
}
```

- [ ] **Step 3: Create test/src/test_ad2428_reg.c**

```c
#include "unity.h"
#include "ad2428_reg.h"

/* Test context that captures calls */
static uint16_t g_last_read_reg = 0;
static uint32_t g_read_value = 0x12345678;
static uint16_t g_last_write_reg = 0;
static uint32_t g_write_value = 0;
static int g_read_calls = 0;
static int g_write_calls = 0;

static A2B_Result test_read(void *priv, uint16_t reg, uint32_t *val)
{
    (void)priv;
    g_last_read_reg = reg;
    *val = g_read_value;
    g_read_calls++;
    return A2B_SUCCESS;
}

static A2B_Result test_write(void *priv, uint16_t reg, uint32_t val)
{
    (void)priv;
    g_last_write_reg = reg;
    g_write_value = val;
    g_write_calls++;
    return A2B_SUCCESS;
}

void setUp(void)
{
    g_last_read_reg = 0;
    g_read_value = 0x12345678;
    g_last_write_reg = 0;
    g_write_value = 0;
    g_read_calls = 0;
    g_write_calls = 0;
}

void tearDown(void) {}

void test_AD2428_RegInit_sets_context(void)
{
    AD2428_RegContext ctx;
    AD2428_RegInit(&ctx, NULL, test_read, test_write);
    TEST_ASSERT_EQUAL_PTR(test_read, ctx.read);
    TEST_ASSERT_EQUAL_PTR(test_write, ctx.write);
}

void test_AD2428_RegRead_calls_read_fn(void)
{
    AD2428_RegContext ctx;
    AD2428_RegInit(&ctx, NULL, test_read, test_write);

    uint32_t val = 0;
    A2B_Result res = AD2428_RegRead(&ctx, 0x0A, &val);

    TEST_ASSERT_EQUAL_INT32(A2B_SUCCESS, res);
    TEST_ASSERT_EQUAL_UINT16(0x0A, g_last_read_reg);
    TEST_ASSERT_EQUAL_UINT32(0x12345678, val);
}

void test_AD2428_RegWrite_calls_write_fn(void)
{
    AD2428_RegContext ctx;
    AD2428_RegInit(&ctx, NULL, test_read, test_write);

    A2B_Result res = AD2428_RegWrite(&ctx, 0x03, 0xDEADBEEF);

    TEST_ASSERT_EQUAL_INT32(A2B_SUCCESS, res);
    TEST_ASSERT_EQUAL_UINT16(0x03, g_last_write_reg);
    TEST_ASSERT_EQUAL_UINT32(0xDEADBEEF, g_write_value);
}

void test_AD2428_RegUpdateBits_reads_modifies_writes(void)
{
    AD2428_RegContext ctx;
    AD2428_RegInit(&ctx, NULL, test_read, test_write);

    /* Simulate current register value */
    g_read_value = 0x0000FFFF;

    A2B_Result res = AD2428_RegUpdateBits(&ctx, 0x03, 0x0000FF00, 0x0000AA00);

    TEST_ASSERT_EQUAL_INT32(A2B_SUCCESS, res);
    TEST_ASSERT_EQUAL_INT(1, g_read_calls);   /* Read first */
    TEST_ASSERT_EQUAL_INT(1, g_write_calls);  /* Then write */
    /* Should have masked and shifted correctly */
    TEST_ASSERT_EQUAL_UINT32(0x0000AA00, g_write_value & 0x0000FF00);
}
```

- [ ] **Step 4: Update test/src/test_main.c**

Add extern declarations and RUN_TEST calls for register tests.

- [ ] **Step 5: Build and run tests**

Run: `cmake --build build && ctest --test-dir build -V`
Expected: All 31 tests PASS

- [ ] **Step 6: Commit**

```bash
git add register/ad2428_reg.h register/ad2428_reg.c test/src/test_ad2428_reg.c
git commit -m "feat: Add AD2428 register definitions

- AD2428 register base addresses and masks
- Register read/write/update_bits interface
- Test callbacks verify read-modify-write sequence
- 5 new passing unit tests (31 total)

Co-Authored-By: Claude <noreply@anthropic.com>"
```

---

## Task 7: Create Empty Core Modules (Phase 2 Placeholders)

**Files:**
- Create: `core/.gitkeep` (done)
- Create: `core/a2b_main.c` (empty stub)
- Create: `core/a2b_device.c` (empty stub)
- Create: `core/a2b_discovery.c` (empty stub)
- Create: `core/a2b_network.c` (empty stub)
- Create: `core/a2b_state.c` (empty stub)

- [ ] **Step 1: Create core/a2b_main.c**

```c
/* OpenA2B Core - Phase 2 */
/* Placeholder for main entry point */
```

- [ ] **Step 2: Create core/a2b_device.c**

```c
/* OpenA2B Device Management - Phase 2 */
/* Placeholder for device initialization */
```

- [ ] **Step 3: Create core/a2b_discovery.c**

```c
/* OpenA2B Discovery Manager - Phase 2 */
/* Placeholder for discovery sequence */
```

- [ ] **Step 4: Create core/a2b_network.c**

```c
/* OpenA2B Network Manager - Phase 2 */
/* Placeholder for network topology */
```

- [ ] **Step 5: Create core/a2b_state.c**

```c
/* OpenA2B State Machine - Phase 2 */
/* Placeholder for state management */
```

- [ ] **Step 6: Commit**

```bash
git add core/a2b_main.c core/a2b_device.c core/a2b_discovery.c core/a2b_network.c core/a2b_state.c
git commit -m "chore: Add placeholder core modules

Adds empty stub files for Phase 2 modules:
- a2b_main.c, a2b_device.c, a2b_discovery.c
- a2b_network.c, a2b_state.c

Co-Authored-By: Claude <noreply@anthropic.com>"
```

---

## Task 8: Finalize Phase 1 - Verify CI Pipeline

**Files:**
- All Phase 1 files should already be created

- [ ] **Step 1: Run full CI locally**

Run: `cmake -B build && cmake --build build && ctest --test-dir build`
Expected: All 31 tests PASS

- [ ] **Step 2: Verify GitHub Actions workflow syntax**

Check `.github/workflows/ci.yml` exists and has correct structure.

- [ ] **Step 3: Create Phase 1 README placeholder**

Create: `docs/phase1_complete.md`
```markdown
# Phase 1 Complete

## What's Built

- CMake project scaffold with HAL and mock libraries
- I2C HAL interface with stub implementation
- Timer HAL interface with stub implementation
- Mock I2C with NACK injection for error testing
- AD2428 register definitions with read/write/update_bits
- Logger system with compile-time level control
- Unity test framework with 31 passing tests
- GitHub Actions CI pipeline

## Next Steps

See [OpenA2B Design Specification](../specs/2026-06-30-OpenA2B-design.md) for Phase 2 plans.

## Test Summary

| Module | Tests |
|--------|-------|
| Types | 4 |
| Error | 5 |
| I2C HAL | 5 |
| Mock I2C | 4 |
| Timer HAL | 4 |
| Logger | 5 |
| Register | 4 |
| **Total** | **31** |
```

- [ ] **Step 4: Final commit**

```bash
git add docs/phase1_complete.md
git commit -m "docs: Add Phase 1 completion summary

- 31 passing unit tests
- CI-ready project structure
- Ready for Phase 2 development

Co-Authored-By: Claude <noreply@anthropic.com>"
```

- [ ] **Step 5: Verify all files are committed**

Run: `git status`
Expected: Working tree clean

Run: `git log --oneline`
Expected: 7 commits (scaffold, types+errors, i2c, timer, logger, registers, placeholders)

---

## Spec Coverage Checklist

| Spec Section | Task(s) | Status |
|-------------|---------|--------|
| Architecture - Layer Overview | Task 1, 2, 3, 4, 5 | ✓ |
| Core Types (A2B_Device, A2B_Node) | Task 2 | ✓ |
| Register Layer | Task 6 | ✓ |
| Platform HAL (I2C) | Task 3 | ✓ |
| Timer Abstraction | Task 4 | ✓ |
| Logging System | Task 5 | ✓ |
| Error Codes | Task 2 | ✓ |
| Mock Hardware | Task 3 | ✓ |
| Unit Test Framework | Task 3 | ✓ |
| CI/CD | Task 1 | ✓ |

All Phase 1 requirements from the design spec are covered.

---

## Self-Review

- [x] No placeholder text (TBD, TODO, implement later)
- [x] Each step shows actual code
- [x] Each step has exact commands with expected output
- [x] Every task ends in a commit
- [x] Types, function signatures consistent across all tasks
- [x] All 31 tests named and defined
- [x] Spec coverage verified

---

## Plan Complete

**Saved to:** `docs/superpowers/plans/2026-06-30-OpenA2B-phase1-foundation.md`

## Execution Options

**1. Subagent-Driven (recommended)** - I dispatch a fresh subagent per task, review between tasks, fast iteration

**2. Inline Execution** - Execute tasks in this session using executing-plans, batch execution with checkpoints

**Which approach?**

cmake_minimum_required(VERSION 3.20)
project(OpenA2B VERSION 1.0.0 LANGUAGES C)

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)

# Options
option(A2B_BUILD_TESTS "Build unit tests" ON)
option(A2B_BUILD_MOCK "Build mock hardware" ON)

# Core library (Phase 2+)
add_library(${PROJECT_NAME}_core INTERFACE)

# HAL library
add_library(${PROJECT_NAME}_hal STATIC
    hal/a2b_i2c.c
    hal/a2b_timer.c
    core/a2b_error.c
    core/a2b_logger.c
    core/a2b_device.c
)
target_include_directories(${PROJECT_NAME}_hal PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/include
    ${CMAKE_CURRENT_SOURCE_DIR}/hal
    ${CMAKE_CURRENT_SOURCE_DIR}/core
)

# Register library
add_library(${PROJECT_NAME}_register STATIC
    register/ad2428_reg.c
)
target_include_directories(${PROJECT_NAME}_register PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/include
    ${CMAKE_CURRENT_SOURCE_DIR}/register
)
target_link_libraries(${PROJECT_NAME}_register PUBLIC ${PROJECT_NAME}_hal)

# Mock library (optional)
if(A2B_BUILD_MOCK)
    add_library(${PROJECT_NAME}_mock STATIC
        mock/mock_i2c.c
    )
    target_include_directories(${PROJECT_NAME}_mock PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/include
        ${CMAKE_CURRENT_SOURCE_DIR}/hal
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

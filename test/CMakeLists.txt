include_directories(
    ${CMAKE_CURRENT_SOURCE_DIR}/../include
    ${CMAKE_CURRENT_SOURCE_DIR}/../hal
    ${CMAKE_CURRENT_SOURCE_DIR}/../mock
    ${CMAKE_CURRENT_SOURCE_DIR}/../register
    ${CMAKE_CURRENT_SOURCE_DIR}/unity
)

set(TEST_SOURCES
    unity/unity.c
    src/test_main.c
    src/test_a2b_types.c
    src/test_a2b_error.c
    src/test_a2b_i2c.c
    src/test_mock_i2c.c
    src/test_a2b_timer.c
    src/test_a2b_logger.c
    src/test_ad2428_reg.c
    src/test_a2b_device.c
    src/test_a2b_state.c
    src/test_a2b_network.c
    src/test_a2b_discovery.c
    src/test_a2b_irq.c
    src/test_a2b_remote.c
    src/test_mock_ad2428.c
    src/test_a2b_audio.c
    src/test_a2b_slot.c
    src/test_a2b_clock.c
    src/test_a2b_diagnostic.c
)

add_executable(${PROJECT_NAME}_tests ${TEST_SOURCES})
target_link_libraries(${PROJECT_NAME}_tests ${PROJECT_NAME}_hal ${PROJECT_NAME}_mock ${PROJECT_NAME}_register)

add_test(NAME ${PROJECT_NAME}_tests COMMAND ${PROJECT_NAME}_tests)
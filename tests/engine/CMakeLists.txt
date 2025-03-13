add_executable(battle_tanks_testing_engine
    ${CMAKE_SOURCE_DIR}/tests/main.cpp
    ConcatTests.cpp
    ClockTests.cpp
    TimerServiceTests.cpp
    TimerTests.cpp
)
target_include_directories(battle_tanks_testing_engine
PRIVATE
    ${CMAKE_SOURCE_DIR}
)

target_link_libraries(battle_tanks_testing_engine 
PRIVATE
    engine_lib
    
    gtest
    gmock

    pthread
    fmt
)

if (CMAKE_BUILD_TYPE STREQUAL "Coverage")
    include(CodeCoverage)
    setup_target_for_coverage(battle_tanks_testing_engine_coverage battle_tanks_testing_engine coverage_engine)
endif() #CMAKE_BUILD_TYPE STREQUAL "Coverage"

add_test(NAME battle_tanks_testing_engine
         COMMAND battle_tanks_testing_engine)

add_executable(battle_tanks_testing_engine
    ${CMAKE_SOURCE_DIR}/tests/main.cpp
    ClockTests.cpp
    ConcatTests.cpp
    MathTests.cpp
    TimerServiceTests.cpp
    TimerTests.cpp
    gui/layout/ConstraintTests.cpp
    gui/layout/GridTests.cpp
    gui/layout/HorizontalTests.cpp
    gui/layout/InsetTests.cpp
    gui/layout/SizeConstraintResolverTests.cpp
    gui/layout/VerticalTests.cpp
    
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
    testing_mocks

    pthread
    fmt
)

if (CMAKE_BUILD_TYPE STREQUAL "Coverage")
    include(CodeCoverage)
    setup_target_for_coverage(battle_tanks_testing_engine_coverage battle_tanks_testing_engine coverage_engine source/engine)
endif() #CMAKE_BUILD_TYPE STREQUAL "Coverage"

add_test(NAME battle_tanks_testing_engine
         COMMAND battle_tanks_testing_engine)

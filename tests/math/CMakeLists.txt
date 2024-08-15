add_executable(battle_tanks_testing_math 
    ${CMAKE_SOURCE_DIR}/tests/main.cpp
    MathTests.cpp
)
target_include_directories(battle_tanks_testing_math
PRIVATE
    ${CMAKE_SOURCE_DIR}
)

target_link_libraries(battle_tanks_testing_math 
PRIVATE
    engine_math_lib

    gtest
    gmock
    pthread

)

if (CMAKE_BUILD_TYPE STREQUAL "Coverage")
    include(CodeCoverage)
    setup_target_for_coverage(battle_tanks_testing_math_coverage battle_tanks_testing_math coverage_math)
endif() #CMAKE_BUILD_TYPE STREQUAL "Coverage"

add_test(NAME battle_tanks_testing_math
         COMMAND battle_tanks_testing_math)

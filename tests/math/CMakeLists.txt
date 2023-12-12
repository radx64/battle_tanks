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
    gtest
    gmock
    pthread

    math_lib
)

add_test(NAME battle_tanks_testing_math
         COMMAND battle_tanks_testing_math)
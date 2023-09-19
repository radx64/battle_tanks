find_package(GTest REQUIRED)

add_executable(battle_tanks_testing 
    main.cpp
    MathTests.cpp
)
target_include_directories(battle_tanks_testing
PRIVATE
    ${CMAKE_SOURCE_DIR}
)

target_link_libraries(battle_tanks_testing 
PRIVATE
    gtest 
    pthread

    game_engine
)

add_test(NAME battle_tanks_testing
         COMMAND battle_tanks_testing)
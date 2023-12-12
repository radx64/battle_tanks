add_executable(battle_tanks_testing_gui
    ${CMAKE_SOURCE_DIR}/tests/main.cpp
    ComponentTests.cpp
)
target_include_directories(battle_tanks_testing_gui
PRIVATE
    ${CMAKE_SOURCE_DIR}
)

target_link_libraries(battle_tanks_testing_gui 
PRIVATE
    gtest
    gmock
    sfml-graphics 
    sfml-window 
    sfml-system
    pthread

    gui_lib
)

add_test(NAME battle_tanks_testing_gui
         COMMAND battle_tanks_testing_gui)
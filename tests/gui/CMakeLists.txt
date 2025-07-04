add_executable(battle_tanks_testing_gui
    ${CMAKE_SOURCE_DIR}/tests/main.cpp
    ButtonTests.cpp
    ComponentTests.cpp
    layout/ConstraintTests.cpp
    layout/SizeConstraintResolverTests.cpp
    layout/InsetTests.cpp
    layout/HorizontalTests.cpp
    layout/VerticalTests.cpp
    layout/GridTests.cpp
)
target_include_directories(battle_tanks_testing_gui
PRIVATE
    ${CMAKE_SOURCE_DIR}
)

target_link_libraries(battle_tanks_testing_gui 
PRIVATE
    gui_lib
    
    gtest
    gmock
    sfml-graphics 
    sfml-window 
    sfml-system
    pthread
    fmt

    testing_mocks
)

if (CMAKE_BUILD_TYPE STREQUAL "Coverage")
    include(CodeCoverage)
    setup_target_for_coverage(battle_tanks_testing_gui_coverage battle_tanks_testing_gui coverage_gui source/gui)
endif() #CMAKE_BUILD_TYPE STREQUAL "Coverage"

add_test(NAME battle_tanks_testing_gui
         COMMAND battle_tanks_testing_gui)

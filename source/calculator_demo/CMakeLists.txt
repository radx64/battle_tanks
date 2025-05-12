add_executable(calculator)

set_target_properties(calculator PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})

target_include_directories(calculator
PUBLIC
    ${CMAKE_SOURCE_DIR}/source
)

target_sources(calculator
PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/Application.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/main.cpp
    )

target_link_libraries(calculator
PUBLIC
    gui_lib
    sfml-system
    sfml-window
    sfml-graphics)

add_executable(physics_sandbox_box2d)

set_target_properties(physics_sandbox_box2d PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})

target_include_directories(physics_sandbox_box2d    
PUBLIC
    ${CMAKE_SOURCE_DIR}/source
)

target_sources(physics_sandbox_box2d
PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/Application.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/main.cpp
    )

target_link_libraries(physics_sandbox_box2d
PUBLIC
    engine_lib
    graphics_lib
    sfml-system
    sfml-window
    sfml-graphics
    box2d)

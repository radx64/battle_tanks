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

# Copy fonts to build directory
file(GLOB_RECURSE RESOURCE_FILES "${CMAKE_SOURCE_DIR}/resources/*")

add_custom_target(copy_resources_calculator ALL
    COMMAND ${CMAKE_COMMAND} -E copy_directory
    ${CMAKE_SOURCE_DIR}/resources $<TARGET_FILE_DIR:calculator>/resources
    DEPENDS ${RESOURCE_FILES}
)
add_dependencies(copy_resources_calculator calculator)

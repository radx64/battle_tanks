add_executable(bios_demo)

set_target_properties(bios_demo PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})

target_include_directories(bios_demo
PUBLIC
    ${CMAKE_SOURCE_DIR}/source
)

target_sources(bios_demo
PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/Application.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/main.cpp
    )

target_link_libraries(bios_demo
PUBLIC
    engine_lib
    sfml-system
    sfml-window
    sfml-graphics)

file(GLOB_RECURSE RESOURCE_FILES "${CMAKE_SOURCE_DIR}/resources/*")

add_custom_target(copy_resources_bios ALL
    COMMAND ${CMAKE_COMMAND} -E copy_directory
    ${CMAKE_SOURCE_DIR}/resources $<TARGET_FILE_DIR:bios_demo>/resources
    DEPENDS ${RESOURCE_FILES}
)
add_dependencies(copy_resources_bios bios_demo)

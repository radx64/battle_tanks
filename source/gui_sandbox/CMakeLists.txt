add_executable(gui_sandbox)

set_target_properties(gui_sandbox PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}
)

option(SANITIZE "Build with sanitizers" OFF)

if (SANITIZE)
    target_compile_options(gui_sandbox PRIVATE -fsanitize=address -fno-omit-frame-pointer)
    target_link_options(gui_sandbox PRIVATE -fsanitize=address)
endif()

target_include_directories(gui_sandbox
PRIVATE
    ${CMAKE_SOURCE_DIR}/source
)

target_sources(gui_sandbox   
PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/Application.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/main.cpp
)

target_link_libraries(gui_sandbox
PRIVATE
    engine_lib
    gui_lib
    sfml-system
    sfml-window
    sfml-graphics)

# Copy resources to build directory
file(GLOB_RECURSE RESOURCE_FILES "${CMAKE_SOURCE_DIR}/resources/*")

add_custom_target(copy_resources_gui_sandbox ALL
    COMMAND ${CMAKE_COMMAND} -E copy_directory
    ${CMAKE_SOURCE_DIR}/resources $<TARGET_FILE_DIR:gui_sandbox>/resources
    DEPENDS ${RESOURCE_FILES}
)
add_dependencies(copy_resources_gui_sandbox gui_sandbox)

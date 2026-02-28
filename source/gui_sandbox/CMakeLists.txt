add_executable(gui_sandbox)

set_target_properties(gui_sandbox PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}
)

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
add_custom_command(TARGET gui_sandbox POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
    ${CMAKE_SOURCE_DIR}/resources $<TARGET_FILE_DIR:gui_sandbox>/resources
)

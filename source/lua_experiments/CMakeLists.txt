add_executable(lua_main)

set_target_properties(lua_main PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})

target_sources(lua_main 
PUBLIC
    main.cpp)
    
target_link_libraries(lua_main
    sol2
    lua::lua
)

# Copy scripts to build directory
add_custom_command(TARGET lua_main POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
    ${CMAKE_SOURCE_DIR}/source/lua_experiments/scripts $<TARGET_FILE_DIR:lua_main>/scripts
)

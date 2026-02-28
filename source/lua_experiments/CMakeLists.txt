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

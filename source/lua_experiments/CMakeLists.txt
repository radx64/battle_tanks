add_executable(lua_main)

target_sources(lua_main 
PUBLIC
    main.cpp)
    
# make sure the interpreter is linked as well; sol2::sol2 depends on lua::lua
# so this is a no‑op when using the alias above, but it's explicit and makes
# the dependency graph clearer.
target_link_libraries(lua_main
    sol2
    lua::lua
)

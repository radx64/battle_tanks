add_executable(physics_sandbox)

set_target_properties(physics_sandbox PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})

target_include_directories(physics_sandbox
PUBLIC
    ${CMAKE_SOURCE_DIR}/source
)

target_sources(physics_sandbox
PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/Application.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/Circle.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/MouseController.hpp
PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/Application.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/main.cpp
    )

target_link_libraries(physics_sandbox
PUBLIC
    engine_lib
    sfml-system
    sfml-window
    sfml-graphics)

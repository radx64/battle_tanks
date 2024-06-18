add_executable(gui_sandbox)

target_include_directories(gui_sandbox
PUBLIC
    ${CMAKE_SOURCE_DIR}/source
)

target_sources(gui_sandbox
PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/Application.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/MouseController.hpp
    
PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/Application.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/main.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/MouseController.cpp
)

target_link_libraries(gui_sandbox
PUBLIC
    engine_lib
    gui_lib
    sfml-system
    sfml-window
    sfml-graphics)
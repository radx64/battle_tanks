add_library(gui_lib STATIC)

target_include_directories(gui_lib
    PUBLIC
    ${CMAKE_SOURCE_DIR}/source
)

target_sources(gui_lib
    PUBLIC
    #${CMAKE_CURRENT_SOURCE_DIR}/xxx.hpp
    PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/dummy.cpp
)
set_property(TARGET gui_lib PROPERTY CXX_STANDARD 17)
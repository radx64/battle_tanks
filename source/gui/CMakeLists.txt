add_library(gui_lib STATIC)

target_include_directories(gui_lib
    PUBLIC
    ${CMAKE_SOURCE_DIR}/source
)

target_sources(gui_lib
    PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/Alignment.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/Element.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/FontLibrary.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/Label.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/StyleSheet.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/Window.hpp    
    PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/FontLibrary.cpp
)
set_property(TARGET gui_lib PROPERTY CXX_STANDARD 17)
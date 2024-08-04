add_library(graphics_lib STATIC)

target_include_directories(graphics_lib
PUBLIC
    ${CMAKE_SOURCE_DIR}/source
)

target_sources(graphics_lib
PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/DrawTools.cpp   
    ${CMAKE_CURRENT_SOURCE_DIR}/TextureLibrary.cpp
)
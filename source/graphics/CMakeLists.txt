add_library(graphics_lib STATIC)

target_include_directories(graphics_lib
    PUBLIC
    ${CMAKE_SOURCE_DIR}/source
)

target_sources(graphics_lib
    PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/DrawTools.hpp    
    ${CMAKE_CURRENT_SOURCE_DIR}/Tilemap.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/TextureLibrary.hpp

    PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/DrawTools.cpp   
    ${CMAKE_CURRENT_SOURCE_DIR}/TextureLibrary.cpp
)
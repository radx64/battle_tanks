add_library(game_engine STATIC)
target_sources(game_engine
    PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/Application.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/Cannon.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/FontLibrary.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/IRenderable.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/Math.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/Tank.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/TextureLibrary.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/Tilemap.hpp
    PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/Application.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/Cannon.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/FontLibrary.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/Math.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/Tank.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/TextureLibrary.cpp
)
set_property(TARGET game_engine PROPERTY CXX_STANDARD 17)
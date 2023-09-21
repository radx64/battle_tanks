add_library(game_lib STATIC)

target_include_directories(game_lib
    PUBLIC
    ${CMAKE_SOURCE_DIR}/source
)

target_sources(game_lib
    PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/Application.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/Cannon.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/Camera.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/Context.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/IRenderable.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/Navigator.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/Tank.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/TankFactory.hpp
    PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/Application.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/Camera.cpp   
    ${CMAKE_CURRENT_SOURCE_DIR}/Cannon.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/Context.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/Tank.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/TankFactory.cpp
)
set_property(TARGET game_lib PROPERTY CXX_STANDARD 17)
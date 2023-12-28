add_library(game_lib STATIC)

target_include_directories(game_lib
    PUBLIC
    ${CMAKE_SOURCE_DIR}/source
)

target_sources(game_lib
    PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/Application.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/Barrel.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/BarrelFactory.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/Cannon.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/Camera.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/Context.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/Crate.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/CrateFactory.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/HelpWindow.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/IRenderable.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/Navigator.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/RigidBody.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/Tank.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/TankFactory.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/Tree.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/TreeFactory.hpp

    PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/Application.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/Barrel.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/BarrelFactory.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/Camera.cpp   
    ${CMAKE_CURRENT_SOURCE_DIR}/Cannon.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/Context.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/Crate.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/CrateFactory.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/Navigator.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/RigidBody.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/Tank.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/TankFactory.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/Tree.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/TreeFactory.cpp
)
add_library(game_lib STATIC)

target_include_directories(game_lib
    PUBLIC
    ${CMAKE_SOURCE_DIR}/source
)

target_link_libraries(game_lib
    PUBLIC 
    engine_lib)

target_sources(game_lib
    PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/Application.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/Context.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/entity/barrel/Barrel.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/entity/barrel/BarrelFactory.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/entity/barrel/BarrelRenderer.hpp 
    ${CMAKE_CURRENT_SOURCE_DIR}/entity/bullet/Bullet.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/entity/bullet/BulletFactory.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/entity/bullet/BulletRenderer.hpp 
    ${CMAKE_CURRENT_SOURCE_DIR}/entity/crate/Crate.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/entity/crate/CrateFactory.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/entity/crate/CrateRenderer.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/entity/tank/Cannon.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/entity/tank/Tank.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/entity/tank/TankFactory.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/entity/tank/TankRenderer.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/entity/TracksRenderer.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/entity/tree/Tree.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/entity/tree/TreeFactory.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/entity/tree/TreeRenderer.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/HelpWindow.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/InstanceIdGenerator.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/MouseController.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/Navigator.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/particle/Explosion.hpp

    PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/Application.cpp 
    ${CMAKE_CURRENT_SOURCE_DIR}/Context.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/entity/barrel/Barrel.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/entity/barrel/BarrelFactory.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/entity/barrel/BarrelRenderer.cpp 
    ${CMAKE_CURRENT_SOURCE_DIR}/entity/bullet/Bullet.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/entity/bullet/BulletFactory.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/entity/bullet/BulletRenderer.cpp 
    ${CMAKE_CURRENT_SOURCE_DIR}/entity/crate/Crate.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/entity/crate/CrateFactory.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/entity/crate/CrateRenderer.cpp   
    ${CMAKE_CURRENT_SOURCE_DIR}/entity/tank/Cannon.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/entity/tank/Tank.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/entity/tank/TankFactory.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/entity/tank/TankRenderer.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/entity/TracksRenderer.cpp     
    ${CMAKE_CURRENT_SOURCE_DIR}/entity/tree/Tree.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/entity/tree/TreeFactory.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/entity/tree/TreeRenderer.cpp 
    ${CMAKE_CURRENT_SOURCE_DIR}/InstanceIdGenerator.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/MouseController.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/Navigator.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/particle/Explosion.cpp
)
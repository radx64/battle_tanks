add_library(game_lib STATIC)

target_include_directories(game_lib
PUBLIC
    ${CMAKE_SOURCE_DIR}/source
)

target_link_libraries(game_lib
PUBLIC 
    engine_lib)

target_sources(game_lib
PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/Application.cpp 
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
    ${CMAKE_CURRENT_SOURCE_DIR}/WaypointMouseController.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/Navigator.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/particle/Explosion.cpp
)

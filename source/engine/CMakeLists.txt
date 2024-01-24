add_library(engine_lib STATIC)

add_subdirectory(math)

target_link_libraries(engine_lib
    PUBLIC
    engine_math_lib)

target_include_directories(engine_lib
    PUBLIC
    ${CMAKE_SOURCE_DIR}/source
)

target_sources(engine_lib
    PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/Camera.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/CollisionSolver.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/FpsCounter.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/FpsLimiter.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/RigidBody.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/GameObject.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/Renderer.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/RigidBody.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/RigidBodyDebugRenderer.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/Scene.hpp

    PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/Camera.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/CollisionSolver.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/FpsCounter.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/FpsLimiter.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/RigidBody.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/GameObject.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/RigidBody.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/RigidBodyDebugRenderer.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/Scene.cpp
)
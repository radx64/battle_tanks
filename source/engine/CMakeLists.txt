add_library(engine_lib STATIC)

target_link_libraries(engine_lib
PUBLIC
    sfml-graphics
    fmt)

target_include_directories(engine_lib
PUBLIC
    ${CMAKE_SOURCE_DIR}/source
)

target_sources(engine_lib
 PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/Application.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/Camera.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/CameraController.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/Context.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/CollisionSolver.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/FpsCounter.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/GameObject.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/math/Math.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/input/KeyboardHandler.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/input/KeyboardReceiver.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/input/MouseHandler.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/input/MouseReceiver.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/input/TextEnteredReceiver.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/Logger.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/LoggerSink.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/Particle.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/ParticleSystem.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/Profiler.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/ProfilingStorage.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/RigidBody.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/RigidBody.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/RigidBodyDebugRenderer.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/Scene.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/Timer.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/TimerService.cpp  
)

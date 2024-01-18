add_library(engine_lib STATIC)

target_include_directories(engine_lib
    PUBLIC
    ${CMAKE_SOURCE_DIR}/source
)

target_sources(engine_lib
    PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/Camera.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/FpsCounter.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/FpsLimiter.hpp

    PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/Camera.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/FpsCounter.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/FpsLimiter.cpp
    )
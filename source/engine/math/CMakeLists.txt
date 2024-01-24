add_library(engine_math_lib STATIC)

target_include_directories(engine_math_lib
    PUBLIC
    ${CMAKE_SOURCE_DIR}/source
)

target_sources(engine_math_lib
    PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/Math.hpp
    PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/Math.cpp
)
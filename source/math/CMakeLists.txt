add_library(math_lib STATIC)

target_include_directories(math_lib
    PUBLIC
    ${CMAKE_SOURCE_DIR}/source
)

target_sources(math_lib
    PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/Math.hpp
    PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/Math.cpp
)
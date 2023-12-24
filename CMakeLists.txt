set(projectName "battle_tanks")
cmake_minimum_required (VERSION 3.0)
project (${projectName})

set(CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/cmake/modules/)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include(CTest)
enable_testing()

find_package(SFML COMPONENTS system window graphics REQUIRED)

if ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU" OR
    "${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang")
    set(warnings "-Wall -Wextra -Werror")
elseif ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "MSVC")
    set(warnings "/W4 /WX /EHsc")
endif()

set(CMAKE_CXX_FLAGS "${warnings} -g -ggdb3"
    CACHE STRING "Flags used by the compiler during all build types." FORCE)
set(CMAKE_C_FLAGS   "${warnings}"
    CACHE STRING "Flags used by the compiler during all build types." FORCE)

option(BUILD_TESTS "Build tests" OFF)

add_subdirectory(source)

if(BUILD_TESTS)
    message(STATUS "Building tests is enabled")
    add_subdirectory(tests)
endif()

add_executable(battle_tanks main.cpp)
target_link_libraries (battle_tanks 
    game_lib
    graphics_lib
    math_lib
    gui_lib
    sfml-graphics 
    sfml-window 
    sfml-system
)

set(CONFIGURED_ONCE TRUE CACHE INTERNAL
    "A flag showing that CMake has configured at least once.")

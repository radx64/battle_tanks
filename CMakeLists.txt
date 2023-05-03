set(projectName "battle_tanks")
cmake_minimum_required (VERSION 3.0)
#set(CMAKE_BUILD_TYPE Debug)
project (${projectName})

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

add_subdirectory (source)

add_executable(battle_tanks main.cpp)
target_link_libraries (battle_tanks game_engine sfml-graphics sfml-window sfml-system)

set(CONFIGURED_ONCE TRUE CACHE INTERNAL
    "A flag showing that CMake has configured at least once.")

set(projectName "battle_tanks")
cmake_minimum_required (VERSION 3.20)
project (${projectName})

set(CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/cmake/modules/)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include(CTest)
enable_testing()

include(FetchContent)

set(FETCHCONTENT_QUIET FALSE)

FetchContent_Declare(
  googletest
  GIT_REPOSITORY https://github.com/google/googletest.git
  GIT_TAG        v1.17.0
  GIT_SHALLOW ON
  GIT_PROGRESS TRUE
  SYSTEM
)

FetchContent_Declare(
  sfml
  GIT_REPOSITORY https://github.com/SFML/SFML.git
  GIT_TAG        2.6.2
  GIT_SHALLOW ON
  GIT_PROGRESS TRUE
  SYSTEM
  OVERRIDE_FIND_PACKAGE
)

FetchContent_Declare(
  fmt
  GIT_REPOSITORY https://github.com/fmtlib/fmt.git
  GIT_TAG        12.1.0 
  GIT_SHALLOW ON
  GIT_PROGRESS TRUE
  SYSTEM
  OVERRIDE_FIND_PACKAGE
)

FetchContent_MakeAvailable(googletest sfml fmt)

find_package(sfml COMPONENTS system window graphics REQUIRED)

if ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU" OR
    "${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang")
    set(warnings "-Wall -Wextra -Werror")
    set(other_flags "-g -ggdb3")
elseif ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "MSVC")
    set(warnings "/W4 /WX /EHsc")
    set(other_flags "/D_USE_MATH_DEFINES")

    # some hack for compiling on Windows with MSVC 
    set(SFML_STATIC_LIBRARIES TRUE)
    set(SFML_DIR "C:/SFML-2.5.1/lib/cmake/SFML")
    set(SFML_INCLUDE_DIR "C:/SFML-2.5.1/include")
    message(CONFIGURE_LOG "SFML include dirs " ${SFML_INCLUDE_DIR})
    include_directories(${SFML_INCLUDE_DIR})
endif()

set(CMAKE_CXX_FLAGS "${warnings} ${other_flags}"
   CACHE STRING "Flags used by the compiler during all build types." FORCE)
set(CMAKE_C_FLAGS   "${warnings} ${other_flags}"
   CACHE STRING "Flags used by the compiler during all build types." FORCE)

option(BUILD_TESTS "Build tests" OFF)

add_subdirectory(source)

if(BUILD_TESTS)
    message(STATUS "Building tests is enabled")
    add_subdirectory(tests)
endif()

add_executable(battle_tanks main.cpp version.rc)
target_link_libraries (battle_tanks
    engine_lib
    game_lib
    graphics_lib
    gui_lib
    sfml-system
    sfml-window
    sfml-graphics
)

# Copy fonts to build directory
add_custom_command(TARGET battle_tanks POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
    ${CMAKE_SOURCE_DIR}/resources $<TARGET_FILE_DIR:battle_tanks>/resources
)

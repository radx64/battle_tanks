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

FetchContent_Declare(
  lua
  GIT_REPOSITORY https://github.com/lua/lua
  # sol2 at the pinned version currently supports up to Lua 5.4; 5.5
  # changes the API and introduces new enums that break the header-only
  # compatibility checks.  Use the last 5.4 release.
  GIT_TAG        v5.4.6
  GIT_SHALLOW ON
  GIT_PROGRESS TRUE
  SYSTEM
  OVERRIDE_FIND_PACKAGE
)

FetchContent_Declare(
  sol2
  GIT_REPOSITORY https://github.com/ThePhD/sol2
  GIT_TAG        v3.3.0
  GIT_SHALLOW ON
  GIT_PROGRESS TRUE
  SYSTEM
  OVERRIDE_FIND_PACKAGE
)

FetchContent_MakeAvailable(googletest sfml fmt lua sol2)

# Lua does not provide CMake build scripts, so after fetching we have to
# build a static library ourselves and expose a target that other parts of
# the project (and sol2) can depend on.  This happens before any other
# subdirectories are added so that the `lua` target is available everywhere.

FetchContent_GetProperties(lua)
if(NOT lua_POPULATED)
    FetchContent_Populate(lua)
endif()

if(NOT TARGET lua)
    # gather the C sources shipped in the repository; there are no headers
    # under a separate src/ directory, everything lives at the top level.
    file(GLOB LUA_C_SOURCES
        "${lua_SOURCE_DIR}/*.c"
    )

    add_library(lua STATIC ${LUA_C_SOURCES})
    target_include_directories(lua PUBLIC ${lua_SOURCE_DIR})
    # make an alias with the conventional namespace so that consumers can
    # write `lua::lua` (the same form used by find_package())
    add_library(lua::lua ALIAS lua)

    # optional: make sure the library is position‑independent (useful if
    # anything is built as a shared object later)
    set_target_properties(lua PROPERTIES POSITION_INDEPENDENT_CODE ON)

    # building the Lua sources leads to a handful of harmless overflow
    # warnings when cross‑compiling with MinGW.  Because we globally add
    # `-Werror` at the top level those warnings become errors, so explicitly
    # turn the error behaviour off for this target instead of trying to
    # enumerate each warning name.  Nobody cares about warnings inside the
    # Lua interpreter itself.
    if (CMAKE_C_COMPILER_ID MATCHES "GNU|Clang")
        target_compile_options(lua PRIVATE -Wno-error)
    endif()
endif()

# We don't automatically link the lua library into sol2 itself because
# sol2's CMake export logic will complain (it tries to export the sol2
# interface target without also exporting lua).  Instead individual
# executables and libraries in this project should link both `sol2` and
# `lua::lua` as needed (see source/lua_experiments/CMakeLists.txt for an
# example).

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

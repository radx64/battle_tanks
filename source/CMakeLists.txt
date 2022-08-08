file(GLOB_RECURSE SRC
    "./*.cpp"
)

include_directories(${CMAKE_CURRENT_SOURCE_DIR})

add_definitions(-std=c++17)
add_definitions(-O3)
add_library(game_engine ${SRC})

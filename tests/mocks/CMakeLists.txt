add_library(testing_mocks STATIC)

target_include_directories(testing_mocks
PRIVATE
    ${CMAKE_SOURCE_DIR}
)

target_link_libraries(testing_mocks
PRIVATE
    gui_lib
    gtest
    gmock
)

target_sources(testing_mocks
PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/ComponentSpy.cpp
)

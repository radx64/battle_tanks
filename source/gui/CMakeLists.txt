add_library(gui_lib STATIC)

target_include_directories(gui_lib
    PUBLIC
    ${CMAKE_SOURCE_DIR}/source
)

target_sources(gui_lib
    PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/Alignment.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/Button.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/Component.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/Event.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/EventReceiver.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/FontLibrary.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/Label.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/Layout.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/StyleSheet.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/Window.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/WindowManager.hpp
    PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/Button.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/Component.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/FontLibrary.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/Label.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/Layout.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/StyleSheet.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/Window.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/WindowManager.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/ProgressBar.cpp

)
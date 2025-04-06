add_library(gui_lib STATIC)

target_include_directories(gui_lib
PUBLIC
    ${CMAKE_SOURCE_DIR}/source
)

target_link_libraries(gui_lib
PUBLIC
    engine_lib
)

target_sources(gui_lib
PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/Alignment.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/Button.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/Component.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/Checkbox.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/Clipboard.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/Debug.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/DimensionConstraintScaler.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/EditBox.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/EventReceiver.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/FontLibrary.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/FontHeightCache.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/FramedSprite.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/KeyboardController.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/keyboard/Utils.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/Label.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/Layout.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/MouseController.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/ProgressBar.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/RadioButton.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/RadioButtonGroup.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/Selection.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/StyleSheet.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/Text.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/TextCursor.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/TextEnteredController.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/Window.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/window/Header.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/window/Panel.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/window/StatusBar.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/WindowManager.cpp
)

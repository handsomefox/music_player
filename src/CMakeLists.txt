set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

include_directories(../vendor/QDarkStyleSheet)

find_package(Qt6 REQUIRED COMPONENTS Widgets Multimedia QuickControls2)

FILE(GLOB UIs *.ui)
qt6_wrap_ui(UI_HEADERS ${UIs})

set(HEADER_FILES
        DirectoryParser.h
        Player.h
        File.h
        UI/MainWindow.h
        Metadata.h
        )

set(SOURCE_FILES
        DirectoryParser.cpp
        Player.cpp
        UI/MainWindow.cpp
        UI/MainWindow.ui
        main.cpp
        ${HEADER_FILES}
        )

SET(CMAKE_INCLUDE_CURRENT_DIR ON)
add_executable(music_player ${SOURCE_FILES})
target_link_libraries(music_player Qt6::Widgets Qt6::Multimedia Qt6::QuickControls2 breeze_themes)
target_precompile_headers(music_player PRIVATE pch.h)

file(COPY ${PROJECT_SOURCE_DIR}/res/cd-drive.png
        DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/res/)

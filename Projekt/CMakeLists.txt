# Minimalna wersja CMake
cmake_minimum_required(VERSION 3.10)

# Definiowanie projektu
project(GUIApp LANGUAGES CXX)

# Włączenie wsparcia dla Qt6 i ustawienia dla automatycznej generacji plików UI, MOC, QRC
find_package(Qt6 REQUIRED COMPONENTS Widgets)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

# Lista plików źródłowych (wszystko w GUIApp)
set(SOURCES
    GUIApp.cpp
    GUIApp.qrc
    DirectoryScanner.cpp
    FileUtils.cpp
    TextAnalyzer.cpp
    
)

set(HEADERS
    GUIApp.h
    DirectoryScanner.h
    FileUtils.h
    TextAnalyzer.h
)
include_directories(build) # Tu podaje folder, w którym zrobię "cmake .." inaczej nie znajduje pliku  ui_GUIApp.h, bo mi nie znajdował go 

# Automatyczne przetwarzanie plików UI i ustawienie ręcznej lokalizacji generacji
qt6_wrap_ui(UI_HEADERS GUIApp.ui)

# Tworzenie pliku wykonywalnego
add_executable(${PROJECT_NAME} ${SOURCES} ${HEADERS} ${UI_HEADERS})

# Linkowanie z Qt6
target_link_libraries(${PROJECT_NAME} Qt6::Widgets)

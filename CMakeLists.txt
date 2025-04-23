cmake_minimum_required(VERSION 3.22)
project(CurrencyConverter)

set(CMAKE_CXX_STANDARD 17)

# Ustaw Qt path – ZMIEN NA SWOJA SCIEZKE, jeśli inna
set(CMAKE_PREFIX_PATH "C:/Qt/6.9.0/mingw_64/lib/cmake")

# Włącz automatyczne przetwarzanie Qt plików
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)

# Znajdź pakiety Qt
find_package(Qt6 REQUIRED COMPONENTS Core Gui Widgets Network)

# Dodaj pliki źródłowe – UWAGA: NIE dodajemy ui_mainwindow.h!
set(SOURCES
        main.cpp
        mainwindow.cpp
        mainwindow.h
        mainwindow.ui
)

# Tworzymy binarkę
add_executable(${PROJECT_NAME} ${SOURCES})

# Linkujemy biblioteki Qt
target_link_libraries(${PROJECT_NAME}
        PRIVATE Qt6::Core Qt6::Gui Qt6::Widgets Qt6::Network
)

# Opcjonalnie – uruchom windeployqt po zbudowaniu
set(WINDEPLOYQT_EXECUTABLE "C:/Qt/6.9.0/mingw_64/bin/windeployqt.exe")
add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
        COMMAND ${WINDEPLOYQT_EXECUTABLE} $<TARGET_FILE:${PROJECT_NAME}>
        COMMENT "Running windeployqt on the executable..."
)

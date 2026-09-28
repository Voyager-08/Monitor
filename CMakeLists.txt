cmake_minimum_required(VERSION 3.16)
project(Monitor LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

find_package(Qt6 REQUIRED COMPONENTS Widgets Quick QuickWidgets)

set(SOURCES
        src/main.cpp
        src/gui/MainWindow.cpp
        src/gui/CommonWidgets.cpp
        src/gui/models/DashboardBackend.cpp
        src/gui/pages/DashboardPage.cpp
        src/gui/pages/SerialPage.cpp
        src/gui/pages/NetworkPage.cpp
        src/gui/pages/ModbusPage.cpp
        src/gui/pages/LogPage.cpp
        src/gui/pages/SettingsPage.cpp
)

set(HEADERS
        include/gui/MainWindow.h
        include/gui/CommonWidgets.h
        include/gui/models/DashboardBackend.h
        include/gui/pages/DashboardPage.h
        include/gui/pages/SerialPage.h
        include/gui/pages/NetworkPage.h
        include/gui/pages/ModbusPage.h
        include/gui/pages/LogPage.h
        include/gui/pages/SettingsPage.h
)

set(RESOURCES
        resources/resources.qrc
)

add_executable(Monitor
        ${SOURCES}
        ${HEADERS}
        ${RESOURCES}
)

target_include_directories(Monitor PRIVATE
        ${CMAKE_SOURCE_DIR}/include
        ${CMAKE_SOURCE_DIR}/src
        ${CMAKE_SOURCE_DIR}/3rdparty/asio/include
        ${CMAKE_SOURCE_DIR}/3rdparty/spdlog/include
        ${CMAKE_SOURCE_DIR}/3rdparty/sqlite
)

target_link_libraries(Monitor PRIVATE
        Qt6::Widgets
        Qt6::Quick
        Qt6::QuickWidgets
)

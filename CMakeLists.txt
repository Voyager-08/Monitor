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
        src/gui/common/CommonWidgets.cpp
        src/gui/models/DashboardBackend.cpp
        src/gui/pages/DashboardPage.cpp
        src/gui/pages/SerialPage.cpp
        src/gui/pages/NetworkPage.cpp
        src/gui/pages/ModbusPage.cpp
        src/gui/pages/LogPage.cpp
        src/gui/pages/SettingsPage.cpp
)

set(HEADERS
        src/gui/MainWindow.h
        src/gui/common/CommonWidgets.h
        include/models/DashboardBackend.h
        include/pages/DashboardPage.h
        include/pages/SerialPage.h
        include/pages/NetworkPage.h
        include/pages/ModbusPage.h
        include/pages/LogPage.h
        include/pages/SettingsPage.h
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

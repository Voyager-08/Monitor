cmake_minimum_required(VERSION 3.16)
project(Monitor LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

find_package(Qt6 REQUIRED COMPONENTS Widgets)

set(SOURCES
        src/main.cpp
        src/gui/MainWindow.cpp
        src/gui/widgets/CommonWidgets.cpp
        src/gui/widgets/DashboardPage.cpp
        src/gui/widgets/SerialPage.cpp
        src/gui/widgets/NetworkPage.cpp
        src/gui/widgets/ModbusPage.cpp
        src/gui/widgets/LogPage.cpp
        src/gui/widgets/SettingsPage.cpp
)

set(HEADERS
        src/gui/MainWindow.h
        src/gui/widgets/CommonWidgets.h
        src/gui/widgets/DashboardPage.h
        src/gui/widgets/SerialPage.h
        src/gui/widgets/NetworkPage.h
        src/gui/widgets/ModbusPage.h
        src/gui/widgets/LogPage.h
        src/gui/widgets/SettingsPage.h
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
        ${CMAKE_SOURCE_DIR}/src
        ${CMAKE_SOURCE_DIR}/3rdparty/asio/include
        ${CMAKE_SOURCE_DIR}/3rdparty/spdlog/include
        ${CMAKE_SOURCE_DIR}/3rdparty/sqlite
)

target_link_libraries(Monitor PRIVATE
        Qt6::Widgets
)

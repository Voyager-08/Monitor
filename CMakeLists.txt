cmake_minimum_required(VERSION 3.16)

project(Monitor VERSION 0.1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

if(NOT CMAKE_BUILD_TYPE AND NOT CMAKE_CONFIGURATION_TYPES)
    set(CMAKE_BUILD_TYPE Release CACHE STRING "Choose the type of build.")
endif()

find_package(Qt6 REQUIRED COMPONENTS Widgets Quick QuickWidgets)

add_executable(Monitor)

target_sources(Monitor PRIVATE
    src/main.cpp
    src/gui/MainWindow.cpp
    src/gui/CommonWidgets.cpp
    src/gui/viewmodels/DashboardBackend.cpp
    src/gui/pages/DashboardPage.cpp
    src/gui/pages/SerialPage.cpp
    src/gui/pages/NetworkPage.cpp
    src/gui/pages/ModbusPage.cpp
    src/gui/pages/LogPage.cpp
    src/gui/pages/SettingsPage.cpp
    src/transport/tcp/TcpClient.cpp
    src/transport/tcp/TcpServer.cpp

    include/gui/MainWindow.h
    include/gui/CommonWidgets.h
    include/gui/viewmodels/DashboardBackend.h
    include/gui/pages/DashboardPage.h
    include/gui/pages/SerialPage.h
    include/gui/pages/NetworkPage.h
    include/gui/pages/ModbusPage.h
    include/gui/pages/LogPage.h
    include/gui/pages/SettingsPage.h
    include/transport/tcp/TcpClient.h
    include/transport/tcp/TcpServer.h

    resources/resources.qrc
)

set_target_properties(Monitor PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin"
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
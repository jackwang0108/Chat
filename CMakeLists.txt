cmake_minimum_required(VERSION 3.14 FATAL_ERROR)

project(
    SimpleC++WebServer
    VERSION 0.0.1
    DESCRIPTION "A Simple C++ Web Server"
    LANGUAGES CXX
)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS ON)
set(EXECUTABLE_OUTPUT_PATH ${CMAKE_SOURCE_DIR}/bin)

# 客户端和服务端通用函数
add_library(
    common
    STATIC
    "lib/common.cpp"
)

target_include_directories(
    common
    PUBLIC "include"
)

# 服务端
add_executable(
    server
    "server.cpp"
    "lib/server.cpp"
)

target_include_directories(
    server
    PRIVATE "include"
)

target_link_libraries(
    server
    pthread
    common
)
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

set(CMAKE_BUILD_TYPE Debug)

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

# 客户端
if(WIN32)
    add_executable(
        client
        "client.cpp"
    )

    # 头文件搜索路径pthread-win32
    target_include_directories(
        client
        PRIVATE "client/pthread/include"
    )

    # 防止windows重定义
    target_compile_definitions(client PRIVATE HAVE_STRUCT_TIMESPEC)

    # 链接静态库
    set(PTHREAD_LIB_PATH "${CMAKE_SOURCE_DIR}/client/pthread/lib/x64")

    target_link_libraries(
        client
        "${PTHREAD_LIB_PATH}/pthreadVC2.lib"
        "${PTHREAD_LIB_PATH}/libpthreadGC2.a"
    )

    # 运行时加载动态库
    set(PTHREAD_DLL_PATH "${CMAKE_SOURCE_DIR}/client/pthread")
    set(WINDOWS_SYSTEM_DLL_PATH "C:/Windows/SysWOW64")

    add_custom_command(TARGET client POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy
        "${PTHREAD_DLL_PATH}/pthreadVC2.dll"
        $<TARGET_FILE_DIR:client>
        COMMAND ${CMAKE_COMMAND} -E copy
        "${WINDOWS_SYSTEM_DLL_PATH}/msvcp140d.dll"
        $<TARGET_FILE_DIR:client>
        COMMAND ${CMAKE_COMMAND} -E copy
        "${WINDOWS_SYSTEM_DLL_PATH}/vcruntime140d.dll"
        $<TARGET_FILE_DIR:client>
    )

    add_custom_target(CopyDLLs ALL DEPENDS
        "${CMAKE_CURRENT_BINARY_DIR}/pthreadVC2.dll"
        "${CMAKE_CURRENT_BINARY_DIR}/msvcp140d.dll"
        "${CMAKE_CURRENT_BINARY_DIR}/vcruntime140d.dll"
    )

    # 在运行时动态加载 DLL
    # configure_file("${PTHREAD_DLL_PATH}/pthreadVC2.dll" "${CMAKE_CURRENT_BINARY_DIR}/pthreadVC2.dll" COPYONLY)
    # configure_file("${WINDOWS_SYSTEM_DLL_PATH}/msvcp140d.dll" "${CMAKE_CURRENT_BINARY_DIR}/msvcp140d.dll" COPYONLY)
    # configure_file("${WINDOWS_SYSTEM_DLL_PATH}/vcruntime140d.dll" "${CMAKE_CURRENT_BINARY_DIR}/vcruntime140d.dll" COPYONLY)
endif()
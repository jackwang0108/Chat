#pragma once

// C++标准库
#include <string>
#include <fstream>
#include <iostream>

// Linux系统库
#include <errno.h>
#include <unistd.h>
#include <stdlib.h>
#include <pthread.h> //g++ -pthread -o server server.cpp
#include <string.h>
#include <stdio.h>
#include <dirent.h>

// Linux系统网络库
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

// 通用工具函数
#include "common.h"

#define QUE_NUM 2         // 最大连接数
#define MAX_BUFF_LEN 1024 // 最大缓冲区长度

// IP和端口设置
#define IP "0.0.0.0"    // 服务器的IP
#define PORT 8000       // 服务器监听端口号
#define FTP_PORT 8006   // FTP端口号
#define VOICE_PORT 8088 // 语音传输端口号

extern int conn_fd[QUE_NUM]; // 所有用户的套接字

struct INFO
{
    // 传递给常驻recv线程的所有参数
    int sock_fd; // 套接字
    int NO;      // 编号
};

struct INFO_SEND
{
    // 传递给send线程的所有参数
    int num_send;
    int dst_sock_fd;           // 目的用户套接字
    int NO;                    // 编号
    char buffer[MAX_BUFF_LEN]; // 待发送内容
};

/**
 * @brief FTP文件传输函数的参数结构体
 */
struct CMD
{
    // 0:offline_send 1:offline_recv 2:online
    int method;
    char filename[MAX_BUFF_LEN + 4];
};

/**
 * @brief 工作线程函数
 * @param arg *INFO, 传递的参数
 * @return
 *
 * @note 每当服务器监听到用户连接, 就会启动一个工作线程进行处理
 */
void *mainWorker(void *arg);
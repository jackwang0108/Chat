
#include "server.h"

using namespace std;

int main()
{
    pthread_t recv_thread0, recv_thread1;
    int sock_fd = socket(AF_INET, SOCK_STREAM, 0);
    int recv_result[QUE_NUM];
    int link_num = 0;                        // 已连接人数
    struct sockaddr_in client_addr[QUE_NUM]; // 客户端地址数组
    INFO info[QUE_NUM];                      // 传递参数的数组

    struct sockaddr_in server_addr;
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(PORT);
    server_addr.sin_addr.s_addr = inet_addr(IP);

    if (bind(sock_fd, (struct sockaddr *)&server_addr, sizeof(server_addr)) == -1)
    {
        cout << "bind fail" << endl;
        cout << errno << endl;
        exit(1);
    }

    if (listen(sock_fd, QUE_NUM) == -1)
    {
        cout << "listen fail" << endl;
        exit(1);
    }

    cout << "Waiting for Connect..." << endl;

    for (link_num = 0; link_num < 2; link_num++)
    {
        socklen_t client_addr_size = sizeof(client_addr[link_num]);
        if ((conn_fd[link_num] = accept(sock_fd, (struct sockaddr *)&client_addr[link_num], &client_addr_size)) == -1)
        {
            cout << "accept fail" << endl;
            exit(1);
        }

        cout << "User " << inet_ntoa(client_addr[link_num].sin_addr) << " Has Connect!" << endl;

        info[link_num].sock_fd = conn_fd[link_num];
        info[link_num].NO = link_num;

        if (link_num == 0)
        {
            recv_result[link_num] = pthread_create(&recv_thread0, NULL, mainWorker, &info[link_num]);
        }
        else if (link_num == 1)
        {
            recv_result[link_num] = pthread_create(&recv_thread1, NULL, mainWorker, &info[link_num]);
        }

        if (recv_result[link_num] != 0)
        {
            cout << "recv_thread create fail" << endl;
            cout << errno << endl;
            exit(1);
        }
    }

    recv_result[0] = pthread_join(recv_thread0, NULL);
    recv_result[1] = pthread_join(recv_thread1, NULL);

    cout << "SYSTEM: Chat over." << endl;

    for (int i = 0; i < 2; i++)
        close(conn_fd[i]);
    close(sock_fd);
    return 0;
}

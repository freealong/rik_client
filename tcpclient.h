#ifndef TCPCLIENT_H
#define TCPCLIENT_H

#include "tcpcommon.h"

#include <QDebug>

const int msg_size = 100;
const int buf_size = 256;

class TcpClient
{
public:
    TcpClient();

    inline void print(const char *m)
    {
        qDebug() << m;
    }
    inline bool is_connected()
    {
        return sockfd > 0;
    }

    void connect_server(const char* address, int port);
    void disconnect_server();
    int download_table(dh_table& t);
    int upload_table(dh_table& t);
    int get_current_pose(Eigen::VectorXf &v);
    int get_current_joints(Eigen::VectorXf &v);
    void test();


private:
    char msg[msg_size];
    char buf[buf_size];
    int sockfd;
};

#endif // TCPCLIENT_H

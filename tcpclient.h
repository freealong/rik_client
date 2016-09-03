#ifndef TCPCLIENT_H
#define TCPCLIENT_H

#include "pose.h"
#include "sharetypes.h"

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
    void download_table(dh_table& t);
    void get_current_pose(Pose& p);

private:
    char msg[msg_size];
    char buf[buf_size];
    int sockfd;
};

#endif // TCPCLIENT_H

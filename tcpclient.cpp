#include "tcpclient.h"

TcpClient::TcpClient() : sockfd(-1)
{
    memset(msg, 0x00, msg_size);
    memset(buf, 0x00, buf_size);
}

void TcpClient::connect_server(const char *address, int port)
{
    struct sockaddr_in serv_addr;
    struct hostent *server;

    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0)
    {
        print("ERROR opening socket");
        return;
    }
    server = gethostbyname(address);
    if (server == NULL) {
        print("ERROR, no such host\n");
        sockfd = -1;
        return;
    }
    bzero((char *) &serv_addr, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    bcopy((char *)server->h_addr,
          (char *)&serv_addr.sin_addr.s_addr,
          server->h_length);
    serv_addr.sin_port = htons(port);
    if (connect(sockfd,(struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0)
    {
        print("ERROR connecting");
        sockfd = -1;
        return;
    }
    print("connect to server successfully.");

    std::string msg("load robot");
    write(sockfd, msg);
    read(sockfd, msg);
    if (msg == "error on load robot")
        print("ERROR on load robot");
    return;
}

void TcpClient::disconnect_server()
{
    close(sockfd);
    sockfd = -1;
}

int TcpClient::download_table(dh_table &t)
{
    if (sockfd == -1)
        return -1;
    std::string msg("download DH table");
    write(sockfd, msg);
    read(sockfd, msg);
    if (msg == "ready on download DH table")
        read(sockfd, t);
    return 0;
}

int TcpClient::upload_table(dh_table &t)
{
    if (sockfd == -1)
        return -1;
    std::string msg("upload DH table");
    write(sockfd, msg);
    write(sockfd, t);
    return 0;
}

int TcpClient::get_current_pose(Eigen::VectorXf &v)
{
    if (sockfd == -1)
        return -1;
    std::string msg("get current pose");
    write(sockfd, msg);
    read(sockfd, v);
    return 0;
}

int TcpClient::get_current_joints(Eigen::VectorXf &v)
{
    if (sockfd == -1)
        return -1;
    std::string msg("get current joints");
    write(sockfd, msg);
    read(sockfd, v);
    return 0;
}

int TcpClient::send_target(float target)
{
    if (sockfd == -1)
        return -1;
    std::string msg("send target");
    write(sockfd, msg);
    write(sockfd, &target, sizeof(target));
    return 0;
}

void TcpClient::test()
{
    std::string msg("test");
    write(sockfd, msg);
    Eigen::VectorXf des;
    read(sockfd, des);
    qDebug() << des(5);
}

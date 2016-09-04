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

void TcpClient::download_table(dh_table &t)
{
    std::string msg("download DH table");
    write(sockfd, msg);
    read(sockfd, msg);
    if (msg == "success on download DH table")
        read(sockfd, t);
    return;
}

void TcpClient::get_current_pose(Pose &p)
{
//    strcpy(msg, "get");
//    int n = write(sockfd, msg, sizeof(msg));
//    if (n < 0) {
//        print("ERROR on write msg");
//    }
//    assert(sizeof(p) < buf_size);
//    n = read(sockfd, buf, sizeof(p));
//    memcpy(&p, buf, sizeof(p));
    return;
}

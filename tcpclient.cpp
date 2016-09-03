#include "tcpclient.h"

#include <cstring>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>

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

//    int n;
//    n = read(sockfd, msg, 100);
//    if (n < 0)
//         print("ERROR reading from socket");
//    print(msg);
    print("connect to server successfully.");
    return;
}

void TcpClient::disconnect_server()
{
    close(sockfd);
    sockfd = -1;
}

void TcpClient::download_table(dh_table &t)
{
//    print("download dh table");
//    t.clear();
//    strcpy(msg, "100");
//    int n = write(sockfd, msg, sizeof(msg));
//    if (n < 0)
//    {
//        print("ERROR on write msg");
//    }
//    dh_parametrs p;
//    assert(sizeof(p) < buf_size);
////    while (1)
//    {
//        print("while");
//        n = read(sockfd, buf, sizeof(p));
////        strncpy(msg, buf, 3);
////        msg[3] = 0;
////        if (strcmp(msg, "101") == 0)
////        {
////            print("end of download1");
////            return;
////        }
//        memcpy(&p, buf, sizeof(p));
////        t.push_back(p);
//    }
//    print("end of download");
//    return;
}

void TcpClient::get_current_pose(Pose &p)
{
    strcpy(msg, "get");
    int n = write(sockfd, msg, sizeof(msg));
    if (n < 0) {
        print("ERROR on write msg");
    }
    assert(sizeof(p) < buf_size);
    n = read(sockfd, buf, sizeof(p));
    memcpy(&p, buf, sizeof(p));
    return;
}

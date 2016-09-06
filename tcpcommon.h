#ifndef TCPCOMMON_H
#define TCPCOMMON_H

#include <cstring>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>

#include <Eigen>
#include "sharetypes.h"

void write(int sockfd, const std::string &str);

void write(int sockfd, const dh_parametrs &p);

void write(int sockfd, const dh_table &t);

void write(int sockfd, const Eigen::VectorXf &v);

void read(int sockfd, std::string &str);

void read(int sockfd, dh_parametrs &p);

void read(int sockfd, dh_table &t);

void read(int sockfd, Eigen::VectorXf &v);


#endif // TCPCOMMON_H

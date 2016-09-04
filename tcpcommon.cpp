#include "tcpcommon.h"

void write(int sockfd, const std::string &str) {
    uint32_t len = str.length();
    write(sockfd, &len, sizeof(len));
    write(sockfd, str.data(), len);
}

void write(int sockfd, const dh_parametrs &p) {
    write(sockfd, &(p.a), sizeof(p.a));
    write(sockfd, &(p.alpha), sizeof(p.alpha));
    write(sockfd, &(p.d), sizeof(p.d));
    write(sockfd, p.joint_name);
    write(sockfd, &(p.z_joint_type), sizeof(p.z_joint_type));
}

void write(int sockfd, const dh_table &t) {
    uint32_t len = t.size();
    write(sockfd, &len, sizeof(len));
    for (dh_table::const_iterator it = t.begin(); it != t.end(); it++) {
        write(sockfd, *it);
    }
}

void read(int sockfd, std::string &str)
{
    uint32_t len;
    char* buf;

    read(sockfd, &len, sizeof(len));
    buf = new char[len];
    read(sockfd, buf, len);
    str.assign(buf, len);
    delete []buf;
}

void read(int sockfd, dh_parametrs &p)
{
    read(sockfd, &(p.a), sizeof(p.a));
    read(sockfd, &(p.alpha), sizeof(p.alpha));
    read(sockfd, &(p.d), sizeof(p.d));
    read(sockfd, p.joint_name);
    read(sockfd, &(p.z_joint_type), sizeof(p.z_joint_type));
}

void read(int sockfd, dh_table &t)
{
    uint32_t len;
    read(sockfd, &len, sizeof(len));
    t.clear();
    dh_parametrs p;
    for (unsigned int i = 0; i < len; ++i)
    {
        read(sockfd, p);
        t.push_back(p);
    }
    t.reserve(len);
}

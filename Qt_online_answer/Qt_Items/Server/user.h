#ifndef USER_H
#define USER_H
#include <string>
#include <tcpsocket.h>


class User
{
public:
    User(std::string n,std::string p,int rank,TcpSocket *s);

    TcpSocket *getSocket();

    const char *getName();

    int getRank();

    int setRank(int rank);

private:
    std::string _usrName;   // 用户名
    std::string _passwd;    // 用户密码
    int _rank;              // 分数

    TcpSocket *_s;          // 通信套接字
};

#endif // USER_H

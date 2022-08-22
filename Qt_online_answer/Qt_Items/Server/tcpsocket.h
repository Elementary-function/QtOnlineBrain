#ifndef TCPSOCKET_H
#define TCPSOCKET_H

#include "tcpserver.h"
#include <string>

class TcpServer;
//通信类，负责与客户端进行通信
class TcpSocket
{
public:
    TcpSocket(TcpServer *tcpServer, struct bufferevent *bev, char *ip, u_int16_t port);

    //在类中使用回调函数必须用static进行修饰
    //可读事件回调函数
    static void readEventCb(struct bufferevent *bev, void *ctx);//ctx就是那个s
    //创建一个通信对象
    //TcpSocket *s = new TcpSocket(this,bev,ip,port);

    //可写事件回调函数
    static void writeEventCb(struct bufferevent *bev, void *ctx);

    //异常事件回调函数
    static void closeEventCb(struct bufferevent *bev,short what, void *ctx);

    char *getIp();   //获取IP地址
    u_int16_t getPort();  //获取端口

    //从客户端读数据
    int readData(void *data,int size);

    //往客户端写数据
    int writeData(const void *data,int size);

    //设置用户名
    void setUserName(std::string name);

    //获取用户名
    std::string getUserName();



private:
    static TcpServer *m_tcpServer; //服务器类对象
    struct bufferevent *m_bev;  //与客户端通信的句柄
    char *m_ip;     //客户端的IP地址
    u_int16_t m_port;   //客户端端口

    std::string _userName;    //当前用户
};

#endif // TCPSOCKET_H

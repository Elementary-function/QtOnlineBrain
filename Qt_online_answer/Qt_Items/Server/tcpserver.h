#ifndef TCPSERVER_H
#define TCPSERVER_H

#include "thread.h"
#include <string.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <event2/listener.h>
#include <event2/bufferevent.h>
#include "tcpsocket.h"
#include "string.h"

class TcpSocket;

//Tcp服务的基类
class TcpServer
{
    friend class TcpSocket;
public:
    TcpServer(int threadNum = 8);

    int listen(int port,const char *ip = NULL);

    //服务器开始运行
    void start();

protected:
    //监听回调函数，有客户端连接时会调用这个函数
    static void listenCb(struct evconnlistener *,evutil_socket_t,struct sockaddr *,int socklen,void *);

    //监听处理函数
    void listenEvent(evutil_socket_t fd,struct sockaddr_in *);

    //----------------虚函数 去具体处理客户端的逻辑----------------
    //客户端连接事件
    virtual void connectEvent(TcpSocket *){}

    //客户端可读
    virtual void readEvent(TcpSocket *){}

    //可写事件
    virtual void writeEvent(TcpSocket *){}

    //关闭事件
    virtual void closeEvent(TcpSocket *,short ){}

private:
    int m_threadNum;         //线程个数
    Thread *m_threadPool;    //线程池

    struct event_base *m_base;
    struct evconnlistener *m_listener;  //监听客户端的连接

    int m_nextThread;        //下一个线程的下标
};

#endif // TCPSERVER_H

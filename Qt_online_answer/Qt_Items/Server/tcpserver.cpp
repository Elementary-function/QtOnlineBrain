#include "tcpserver.h"

TcpServer::TcpServer(int threadNum):m_nextThread(0)
{
    if(threadNum <= 0)
    {
        printf("threadNum <= 0\n");
        exit(-1);
    }
    //创建线程池
    m_threadNum = threadNum;
    m_threadPool = new Thread[m_threadNum];
    if(m_threadPool == NULL)
    {
        printf("create threadPool error!\n");
        exit(-1);
    }
    m_base = event_base_new();
    if(!m_base)
    {
        printf("Clouldn't create an event_base:exiting\n");
        exit(-1);
    }
}

void TcpServer::listenCb(struct evconnlistener *,evutil_socket_t fd,struct sockaddr *clientAdd,int ,void *date)
{
    //第一个参数是listen的监听事件
    //第二个参数与客户端通信的文件描述符
    //第三个参数客户端的IP地址
    //第五个参数是回调函数参数
    TcpServer *p = (TcpServer *)date;
    p->listenEvent(fd,(struct sockaddr_in *)clientAdd);

}
void TcpServer::listenEvent(evutil_socket_t fd,struct sockaddr_in *clientAdd)
{
    char *ip = inet_ntoa(clientAdd->sin_addr);    //客户端的IP地址
    uint16_t port = ntohs(clientAdd->sin_port);   //客户端使用的端口
    //从线程池中选择一个线程去处理客户端的请求
    //以轮询的方式选择线程

    struct event_base *base = m_threadPool[m_nextThread].getBase();
    m_nextThread = (m_nextThread+1) % m_threadNum; //让你线程轮转

    struct bufferevent *bev = bufferevent_socket_new(base,fd,BEV_OPT_CLOSE_ON_FREE);
    //功能:创建一个用于socket的bufferevent，用来缓存套接字读或写的数据，然后调用对应的回调函数
    //base:表示event_base
    //options:是bufferevent选项的位掩（BEV_OPT_CLOSE_ON_FREE等）。
    //fd:参数是一个可选的socket文件描述符。如果希望以后再设置socket文件描述符，可以将fd置为-1。
    //bev客户端句柄
    if(!bev)
    {
        printf("Error constructing bufferevent!");
        event_base_loopbreak(base);
        return;
    }
    //创建一个通信对象
    TcpSocket *s = new TcpSocket(this,bev,ip,port);
    //单独封装一个类负责和客户端的通信
    bufferevent_setcb(bev,s->readEventCb,s->writeEventCb,s->closeEventCb,s);
    //该函数的作用主要是赋值，把该函数后面的参数，赋值给第一个参数struct bufferevent *bufev定义的变量
    bufferevent_enable(bev, EV_WRITE);
    bufferevent_enable(bev, EV_READ);
    bufferevent_enable(bev, EV_SIGNAL);//打开（读、写、信号）三个开关

    //调用客户端连接事件
    connectEvent(s);

}

int TcpServer::listen(int port,const char *ip)
{
    struct sockaddr_in sin;
    memset(&sin,0,sizeof(sin));
    /*
    sin_family主要用来定义是哪种地址族

    sin_port主要用来保存端口号

    sin_addr主要用来保存IP地址信息
    */
    sin.sin_family = AF_INET;  //指定IP地址地址版本人为IPV4
    sin.sin_port = htons(port);  //存放端口信息
    if(ip != NULL)
    {
        //功能：将一个字符串表示的点分十进制IP地址IP转换为网络字节序存储在addr中，并且返回该网络字节序表示的无符号整数。
        inet_aton(ip,&sin.sin_addr);
    }
    //创建监听事件
    m_listener = evconnlistener_new_bind(m_base,listenCb,this,
                  LEV_OPT_REUSEABLE|LEV_OPT_CLOSE_ON_FREE,-1,
                  (struct sockaddr*)&sin,sizeof(sin));
    /*
    base： event_base对象。参考event_base。
    cb： 回调函数， 当有新的TCP连接发生时，会唤醒回调函数。
    ptr： 传递给回调函数的参数。

    flags： 一些标志， 后面会进一步介绍。
    //LEV_OPT_CLOSE_ON_FREE：释放listener时，先关闭潜在的套接字
    //LEV_OPT_REUSEABLE：一些系统平台在默认情况下即使已经关闭了socket，也要等待一段时间才能使用对应的port，
    //而这个flag则设置成只要已关闭socket就是重新使用对应的port
    backlog： 监听队列允许容纳的最大连接数。

    sa： evconnlistener_new_bind帮助我们绑定监听地址。sa就是传入的监听地址。
    socklen： sa的长度。
    */
    if(!m_listener)
    {
        printf("Couldn't create alistener\n");
        exit(1);
        return -1;
    }

    //开启线程池
    for(int i = 0;i <m_threadNum;i++)
    {
        m_threadPool[i].start();
        printf("线程 %d 启动\n",i+1);
    }
    //spdlog::info("Welcome to spdlog!");
    return 0;
}

void TcpServer::start()
{
    event_base_dispatch(m_base);
    evconnlistener_free(m_listener);
    event_base_free(m_base);

    printf("done\n");
}

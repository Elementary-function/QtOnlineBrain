#include "thread.h"

Thread::Thread()
{
    m_base = event_base_new();
    if(!m_base)
    {
        printf("Clouldn't create an event_base:exiting\n");
        exit(-1);
    }
    //创建管道
    int fd[2];
    if(pipe(fd) == -1)
    {
        perror("pipe");
        exit(-1);
    }

    m_pipeReadFd = fd[0];
    m_pipWirteFd = fd[1];

    //让管道事件监听管道读端
    //如果监听到 管道的读端有数据可读
    event_set(&m_pipeEvent,m_pipeReadFd,EV_READ | EV_PERSIST,pipeRead,this);
            //这就是主线程，不仅可以监听，而且可以防止base里面事件不为空;

    //将事件添加到m_base集合中
    event_base_set(m_base,&m_pipeEvent);

    //开启事件的监听
    event_add(&m_pipeEvent,0);
}

void Thread::pipeRead(evutil_socket_t,short,void *)
{

}

void Thread::start()
{
    //创建一个线程
    pthread_create(&m_threadId,NULL,worker,this);
    //第一参数线程ID
    //属性一般为NULL
    //线程函数（线程要去执行的函数）
    //传给线程函数的参数

    //线程分离
    pthread_detach(m_threadId);
}

void* Thread::worker(void *arg)
{
    Thread *p = (Thread *)arg;
    p->run();
   // sleep(10);
    return NULL;
}

 void Thread::run()
 {
//     while(1)
//     {
//         printf("thread = %d\n",m_threadId);
//         sleep(2);
//     }
     //printf("%d:start",m_threadId);
     //监听base事件集合  event_base_dispatch 死循环 类似Qt 的exec()
     //如果 m_base 事件集合内部是空的花，则event_base_dispatch会立马返回
     //初始化的时候，需要给m_base
     event_base_dispatch(m_base);
     event_base_free(m_base);
     printf("%d:close",m_threadId);

     return ;
 }

 struct event_base *Thread::getBase()
 {
     return m_base;
 }

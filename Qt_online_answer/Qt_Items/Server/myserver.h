#ifndef MYSERVER_H
#define MYSERVER_H

#include "tcpserver.h"
#include <spdlog/spdlog.h>
#include "db.h"
#include "json/json.h"
#include "json/reader.h"
#include "../common.h"
#include <string>
#include <map>
#include "user.h"
#include "spdlog/sinks/stdout_color_sinks.h"

#define DEBUG
#define QUESTION_NUM 5

class Myserver:public TcpServer
{
public:
    Myserver();
    //客户端连接事件
    void connectEvent(TcpSocket *);

    //客户端可读
    void readEvent(TcpSocket *s);

    //可写事件
    void writeEvent(TcpSocket *);

    //关闭事件
    void closeEvent(TcpSocket *,short);

private:
    //发送数据
    void writeData(TcpSocket *s,const Json::Value &inJson);

    //用户注册
    void Register(TcpSocket *s,const Json::Value &inJson);

    //用户登陆
    void Login(TcpSocket *s,const Json::Value &inJson);
    
    //个人训练获取问题
    void singleGetQuestion(TcpSocket *s);

    //初始化段位列表
    void initRankMap();

    //排位
    void Rank(TcpSocket *s);

    //开始对决
    void startRank(TcpSocket *first,TcpSocket *second);

    //rank回答一个问题
    void RankAnswerQuestion(TcpSocket *s,const Json::Value &inJson);

    //rank结果
    void RankResult(TcpSocket *s,const Json::Value &inJson);

private:
    std::shared_ptr<spdlog::logger> _log;   //记录日志的句柄
    DB *_db;   //数据库句柄

    //健是用户名，值是用户指针
    std::mutex _userLock;                   //锁，锁_users的
    std::map<std::string, User*> _users;    //在线用户列表

    //key  rank积分
    //值   对应的段位说明
    std::map<int,std::string> _rankMap;     //段位列表

    //key  rank积分
    //值   等待rank的用户
    std::mutex _rankLock;
    std::map<int, TcpSocket*> _rankQueue;   //等待排位列表
};

#endif // MYSERVER_H

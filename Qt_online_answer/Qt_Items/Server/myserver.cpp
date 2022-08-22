#include "myserver.h"

Myserver::Myserver()
{
#ifdef DEBUG
    //_log = spdlog::stdout_color_mt("log");
    auto console = spdlog::stdout_color_mt("log");
    //函数创建一个名字为_log的console logger，
    //把这个logger注册到spdlog的全局注册表中，并且返回指向这个logger的指针（shared_ptr）。
#else
    _log = spdlog::rotating_logger_mt("log","log",1024*1024 * 5,3);
    /*
    rotating_logger_mt
    第一个参数是句柄名称
    第二个参数是文件名称
    第三个是日志大小
    第四个参数是多少个文件进行循环
    */
    _log->flush_on(spdlog::level::info);  //显示的权限等级

#endif
    _db = new DB(NULL,"qt","qt","qt_online_answer");
    initRankMap();

}
//客户端连接事件
void Myserver::connectEvent(TcpSocket *s)
{
    spdlog::get("log")->info("有一个新连接：[{}:{}]",s->getIp(),s->getPort());
}

//客户端可读
void Myserver::readEvent(TcpSocket *s)
{
    char buf[1024] = {0};
    memset(buf,0,sizeof(buf));
    while(1)
    {
        int len = 0;
        s->readData(&len,sizeof(len));
        if(len <= 0)
        {
            break;
        }
        s->readData(buf,len);

        //数据解析
        Json::Value root;
        Json::Reader reader;  //json解析器
        //将buf转化成root形式
        if(!reader.parse(buf,root))
        {
            spdlog::get("log")->error("json 数据解析失败");
            return;
        }

        int cmd = root["cmd"].asInt();
        switch (cmd)
        {
        case REGISTER:
            Register(s,root);
            break;
        case LOGIN:
            Login(s,root);
            break;
        case SINGLEGETQUESTION:
            singleGetQuestion(s);
            break;
        case RANK:
            Rank(s);
            break;
        case ANSWER:
            RankAnswerQuestion(s,root);
            break;
        case RANKRESULT:
            RankResult(s,root);
            break;
        default:
            break;
        }

    }
}

//可写事件
void Myserver::writeEvent(TcpSocket *)
{

}

//关闭事件
void Myserver::closeEvent(TcpSocket *s,short)
{
    {
        std::unique_lock<std::mutex> lock(_rankLock);
        int rank = _users[s->getUserName()]->getRank();
        auto it = _rankQueue.find(rank);
        if(it != _rankQueue.end())
        {
                _rankQueue.erase(it);
        }
    }
    std::unique_lock<std::mutex> lock(_userLock);
    std::map<std::string, User*>::iterator it = _users.begin();
    while (it != _users.end())
    {
        if(it->second->getSocket() == s)
        {
            _users.erase(it);
            spdlog::get("log")->info("用户{}[{}:{}] log out",it->second->getName(),s->getIp(),s->getPort());
            //释放User
            delete it->second;
            return;
        }
        it++;
    }
    spdlog::get("log")->info("[{}:{}] log out",s->getIp(),s->getPort());
}

void Myserver::writeData(TcpSocket *s,const Json::Value &inJson)
{
    std::string data = inJson.toStyledString();
    s->writeData(data.c_str(),data.length());
}

void Myserver::Register(TcpSocket *s,const Json::Value &inJson)
{
    std::string userName = inJson["userName"].asString();
    std::string passwd = inJson["passwd"].asString();

    //检测用户是否存在
    char sql[1024] = {0};
    sprintf(sql,"select * from user where name = '%s' and passwd = '%s'",userName.c_str(),passwd.c_str());

    int result = OK;
    Json::Value outJson;
    bool ret = _db->db_select(sql, outJson);
    if(!ret)
    {
        result = ERROR;
        spdlog::get("log")->error("Register select user error");
    }

    if(outJson.isMember("name"))   //用户存在，表明已经注册过了
    {
        result = USEREXIST;
    }
    else
    {
        memset(sql,0,sizeof(sql));
        sprintf(sql,"insert into user(name,passwd,rank) values('%s','%s',0)",userName.c_str(),passwd.c_str());
        bool ret = _db->db_select(sql, outJson);
        if(!ret)
        {
            result = ERROR;
            spdlog::get("log")->error("Register insert user error");
        }
        else
        {
            spdlog::get("log")->info("Register user = {} success",userName);
        }
    }
    Json::Value json;
    json["cmd"]    = REGISTER;
    json["result"] = result;

    writeData(s,json);
}

void Myserver::Login(TcpSocket *s,const Json::Value &inJson)
{
    std::string userName = inJson["userName"].asString();
    std::string passwd = inJson["passwd"].asString();
    //int rank = inJson["rank"].asInt();
    int i = 0;
    int rank;
    //检测用户是否已经注册
    char sql[1024] = {0};
    sprintf(sql,"select * from user where name = '%s' and passwd = '%s'",userName.c_str(),passwd.c_str());

    int result = OK;
    Json::Value outJson;
    bool ret = _db->db_select(sql, outJson);
    if(!ret)
    {
        result = ERROR;
        spdlog::get("log")->error("Login select user error");
    }

    if(outJson.isMember("name"))   //用户存在，表明已经注册过了
    {
        std::unique_lock<std::mutex> lock(_userLock);
        if(_users.find(userName) != _users.end())  //用户已经登陆
        {
            result = USERLOGIN;
            spdlog::get("log")->info("用户{}[{}:{}] 已经登陆",userName,s->getIp(),s->getPort());
        }
        else
        {
            rank = atoi(outJson["rank"][i].asString().c_str());
            User* user = new User(userName,passwd,rank,s);
            _users.insert(make_pair(userName,user));
            s->setUserName(userName);
            spdlog::get("log")->info("用户{}[{}:{}] login",userName,s->getIp(),s->getPort());
        }
    }
    else
    {
        result = NAMEORPASSWD;
    }
    Json::Value json;
    json["cmd"]      = LOGIN;
    json["result"]   = result;
    json["userName"] = userName;
    json["rank"]     = rank;
    json["grade"]    = _rankMap[rank];
    //spdlog::get("log")->info("用户rank:{}\n",rank);

    writeData(s,json);
}

void Myserver::singleGetQuestion(TcpSocket *s)
{
    char sql[1024] = {0};
    memset(sql,0,sizeof(sql));
    sprintf(sql,"select * from question order by rand() limit %d",QUESTION_NUM);

    int result = OK;
    Json::Value outJson;
    bool ret = _db->db_select(sql, outJson);
    if(!ret)
    {
        result = ERROR;
        spdlog::get("log")->error("Login select user error");
    }
    Json::Value json;
    json["cmd"]      = SINGLEGETQUESTION;
    json["result"]   = result;
    json["question"] = outJson;
    spdlog::get("log")->info("用户{}[{}:{}] 获取题目：{}\n",s->getUserName(),s->getIp(),s->getPort(),json.toStyledString());

    writeData(s,json);
}
//-----------------rank-------------------

//初始化段位列表
void Myserver::initRankMap()
{
    char buf[100];
    int rank = 0;
    int star = 0;
    for(int i = 0;i < 100; i++)
    {
        memset(buf,0,sizeof(buf));
        if(i < 9)
        {
            rank = i / 3;
            star = i % 3;
            sprintf(buf,"青铜%d %d颗星", 3-rank, star+1);
        }
        else if(i < 18)
        {
            rank = (i - 9) / 3;
            star = (i - 9) % 3;
            sprintf(buf,"白银%d %d颗星", 3-rank, star+1);
        }
        else if(i < 34)
        {
            rank = (i - 18) / 4;
            star = (i - 18) % 4;
            sprintf(buf,"黄金%d %d颗星", 4-rank, star+1);
        }
        else if(i < 50)
        {
            rank = (i - 34) / 4;
            star = (i - 34) % 4;
            sprintf(buf,"铂金%d %d颗星", 4-rank, star+1);
        }
        else if(i < 75)
        {
            rank = (i - 50) / 5;
            star = (i - 50) % 5;
            sprintf(buf,"钻石%d %d颗星", 5-rank, star+1);
        }
        else if(i < 100)
        {
            rank = (i - 75) / 5;
            star = (i - 75) % 5;
            sprintf(buf,"星曜%d %d颗星", 5-rank, star+1);
        }
        _rankMap.insert(std::make_pair(i,buf));

    }
//    for(int i = 0;i < 100; i++)
//    {
//        std::cout<< i <<":"<<_rankMap[i]<<std::endl;
//    }
}

//rank回答一个问题
void Myserver::RankAnswerQuestion(TcpSocket *s,const Json::Value &inJson)
{
    std::string enemyName = inJson["enemyName"].asString();
    User* user = _users[enemyName];

    Json::Value json;
    json["cmd"] = ANSWER;
    json["enemyScore"] = inJson["score"].asInt();
    json["enemyQuestionId"] = inJson["questionId"].asInt();
    //spdlog::get("log")->info("cmd:{}enemyScore:{}enemyQuestionId:{}",ANSWER,inJson["score"].asInt(),inJson["questionId"].asInt());

    writeData(user->getSocket(),json);
}

//开始排位
void Myserver::Rank(TcpSocket *s)
{
    TcpSocket *other = NULL;   //对手
    int rank = _users[s->getUserName()]->getRank();    //当前用户 rank 积分

    std::unique_lock<std::mutex> lock(_rankLock);
    //查找同一段位的对手
    std::map<int,TcpSocket*>::iterator it = _rankQueue.find(rank);
    if(it != _rankQueue.end())
    {
        other = it->second;
        _rankQueue.erase(it);
    }
    else
    {
        //查找其他段位的选手  积分差值的绝对值小于5的都可以进行对决
        for(int i = 1;i <= 5; i++)
        {
            it = _rankQueue.find(rank + i);
            if(it != _rankQueue.end())
            {
                //spdlog::get("log")->info("it->first:{}\n",it->first);
                other = it->second;
                _rankQueue.erase(it);
                break;
            }
            it = _rankQueue.find(rank - i);
            if(it != _rankQueue.end())
            {
                //spdlog::get("log")->info("it->first:{}\n",it->first);
                other = it->second;
                _rankQueue.erase(it);
                break;
            }
        }
    }
    spdlog::get("log")->info("当前分数：{}",rank);
    if(other == NULL) //没有匹配到用户
    {
        _rankQueue.insert(std::make_pair(rank,s));
        spdlog::get("log")->info("当前等候 rank 人数：{}",_rankQueue.size());
    }
    else //找到
    {
        startRank(s,other);
        //开始对决
    }


}

//开始对决
void Myserver::startRank(TcpSocket *first,TcpSocket *second)
{
    char sql[1024] = {0};
    memset(sql,0,sizeof(sql));
    sprintf(sql,"select * from question order by rand() limit %d",QUESTION_NUM);

    int result = OK;
    Json::Value outJson;
    bool ret = _db->db_select(sql, outJson);
    if(!ret)
    {
        result = ERROR;
        spdlog::get("log")->error("startRank select question error");
    }
    Json::Value json;
    json["cmd"]      = RANK;
    json["result"]   = result;
    json["question"] = outJson;

    //first user
    json["enemyName"] = second->getUserName();
    json["enemyRank"] = _rankMap[_users[second->getUserName()]->getRank()];
    json["enemyScore"] = 0;
    //spdlog::get("log")->info("enemyName = {},enemyRank = {},enemyScore = {}",second->getUserName(),_rankMap[_users[second->getUserName()]->getRank()],_users[second->getUserName()]->getRank());
    writeData(first,json);

    //second user
    json["enemyName"] = first->getUserName();
    json["enemyRank"] = _rankMap[_users[first->getUserName()]->getRank()];
    writeData(second,json);

    spdlog::get("log")->info("获取题目：{}\n",json.toStyledString());
}

//rank结果
void Myserver::RankResult(TcpSocket *s,const Json::Value &inJson)
{
    std::unique_lock<std::mutex> lock(_userLock);
    User *user = _users[s->getUserName()];

    int score = inJson["score"].asInt();
    int enemyScore = inJson["enemyScore"].asInt();

    if(score < enemyScore)
    {
        user->setRank(user->getRank() - 1);
    }
    else if(score > enemyScore)
    {
        user->setRank(user->getRank() + 1);
    }

    Json::Value json;
    json["cmd"] = RANKRESULT;
    json["newRank"] = _rankMap[user->getRank()];
    writeData(s,json);

}

#include "db.h"

DB::DB(const char *host, const char *usrName, const char *passwd, const char *dbName)
{
    //初始化数据库句柄
    _mysql = mysql_init(NULL);
    if(_mysql == NULL)
    {
        spdlog::get("log")->error("mysql init error\n");
        exit(-1);
    }
    //连接数据库服务器
    MYSQL *con = mysql_real_connect(_mysql,host,usrName,passwd,dbName,0,NULL,0);
    //第一个参数是数据库句柄
    //第二个参数是主机host
    //第三个参数是用户名
    //第四个参数是密码
    //第五个参数是所要打开的数据库，后面的参数就不需要了
    if(con ==NULL)
    {
        spdlog::get("log")->error("数据库连接失败：{}",mysql_error(_mysql));
        exit(-1);
    }
    _mysql = con;
    //设置字符集，使其支持中文
    int ret = mysql_query(_mysql,"set names utf8");
    if(ret != 0)
    {
        spdlog::get("log")->error("设置字符集失败：{}",mysql_error(_mysql));
        exit(-1);
    }

}
bool DB::db_exec(const char *sql)
{
    std::unique_lock<std::mutex> loc(_mutex);   //数据库句柄上锁
    int ret = mysql_query(_mysql,sql);
    //mysql_query() 函数执行一条 MySQL 查询。
    if(ret != 0)
    {
        spdlog::get("log")->error("设置字符集失败：{}",mysql_error(_mysql));
        return false;
    }
    return true;
}
bool DB::db_select(const char *sql, Json::Value &outJson)
{
    std::unique_lock<std::mutex> loc(_mutex);   //数据库句柄上锁
    int ret = mysql_query(_mysql,sql);
    //mysql_query() 函数执行一条 MySQL 查询。
    if(ret != 0)
    {
        spdlog::get("log")->error ("mysql_query error：%s",mysql_error(_mysql));
        return false;
    }
    //从mysql服务器下载查询结果
    MYSQL_RES *sql_res = mysql_store_result(_mysql);
    if(NULL == sql_res)
    {
        if(mysql_errno(_mysql) == 0)
        {
            return true;
        }
        else
        {
            spdlog::get("log")->error ("mysql_store_result error：%s",mysql_error(_mysql));
        }
    }

    MYSQL_ROW row; //从结果集中一行一行的取出数据
    unsigned int num_fields = mysql_num_fields(sql_res); //获取列数
    MYSQL_FIELD *fetch_field = mysql_fetch_field(sql_res);  //获取表头
    //一行一行的获取数据
    while(row = mysql_fetch_row(sql_res))
    {
        for(unsigned int i = 0;i < num_fields; i++)
        {
            outJson[fetch_field[i].name].append(row[i]);
        }
    }
    mysql_free_result(sql_res);
    return true;
}


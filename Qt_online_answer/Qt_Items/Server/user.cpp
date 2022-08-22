#include "user.h"

User::User(std::string n,std::string p,int rank,TcpSocket *s):
    _usrName(n),
_passwd(p),
_rank(rank),
_s(s)
{

}

TcpSocket *User::getSocket()
{
    return _s;
}

const char *User::getName()
{
    return _usrName.c_str();
}

int User::getRank()
{
    return _rank;
}

int User::setRank(int rank)
{
    if(rank <= 0)
    {
        _rank = 0;
    }
    _rank = rank;
}

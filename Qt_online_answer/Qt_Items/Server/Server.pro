TEMPLATE = app
CONFIG += console c++11
CONFIG -= app_bundle
CONFIG -= qt
CONFIG += C++11

SOURCES += main.cpp \
    thread.cpp \
    tcpserver.cpp \
    tcpsocket.cpp \
    myserver.cpp \
    db.cpp \
    user.cpp

HEADERS += \
    thread.h \
    tcpserver.h \
    tcpsocket.h \
    myserver.h \
    db.h \
    user.h

//给linevent添加库（以及线程）
LIBS += -L/usr/local/lib -levent -lpthread -ljson -lmysqlclient
LIBS += -L/usr/lib/ -lspdlog

/****************************************************************************
** Meta object code from reading C++ file 'communicate.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.9.0)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../client/communicate.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'communicate.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.9.0. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_Communicate_t {
    QByteArrayData data[8];
    char stringdata0[70];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_Communicate_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_Communicate_t qt_meta_stringdata_Communicate = {
    {
QT_MOC_LITERAL(0, 0, 11), // "Communicate"
QT_MOC_LITERAL(1, 12, 9), // "rstResult"
QT_MOC_LITERAL(2, 22, 0), // ""
QT_MOC_LITERAL(3, 23, 5), // "Login"
QT_MOC_LITERAL(4, 29, 4), // "json"
QT_MOC_LITERAL(5, 34, 21), // "receiveSingleQuestion"
QT_MOC_LITERAL(6, 56, 4), // "Rank"
QT_MOC_LITERAL(7, 61, 8) // "readData"

    },
    "Communicate\0rstResult\0\0Login\0json\0"
    "receiveSingleQuestion\0Rank\0readData"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_Communicate[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       5,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       4,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    1,   39,    2, 0x06 /* Public */,
       3,    1,   42,    2, 0x06 /* Public */,
       5,    1,   45,    2, 0x06 /* Public */,
       6,    1,   48,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
       7,    0,   51,    2, 0x0a /* Public */,

 // signals: parameters
    QMetaType::Void, QMetaType::Int,    2,
    QMetaType::Void, QMetaType::QJsonObject,    4,
    QMetaType::Void, QMetaType::QJsonObject,    2,
    QMetaType::Void, QMetaType::QJsonObject,    2,

 // slots: parameters
    QMetaType::Void,

       0        // eod
};

void Communicate::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Communicate *_t = static_cast<Communicate *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->rstResult((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 1: _t->Login((*reinterpret_cast< QJsonObject(*)>(_a[1]))); break;
        case 2: _t->receiveSingleQuestion((*reinterpret_cast< QJsonObject(*)>(_a[1]))); break;
        case 3: _t->Rank((*reinterpret_cast< QJsonObject(*)>(_a[1]))); break;
        case 4: _t->readData(); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        void **func = reinterpret_cast<void **>(_a[1]);
        {
            typedef void (Communicate::*_t)(int );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&Communicate::rstResult)) {
                *result = 0;
                return;
            }
        }
        {
            typedef void (Communicate::*_t)(QJsonObject );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&Communicate::Login)) {
                *result = 1;
                return;
            }
        }
        {
            typedef void (Communicate::*_t)(QJsonObject );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&Communicate::receiveSingleQuestion)) {
                *result = 2;
                return;
            }
        }
        {
            typedef void (Communicate::*_t)(QJsonObject );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&Communicate::Rank)) {
                *result = 3;
                return;
            }
        }
    }
}

const QMetaObject Communicate::staticMetaObject = {
    { &QObject::staticMetaObject, qt_meta_stringdata_Communicate.data,
      qt_meta_data_Communicate,  qt_static_metacall, nullptr, nullptr}
};


const QMetaObject *Communicate::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *Communicate::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_Communicate.stringdata0))
        return static_cast<void*>(const_cast< Communicate*>(this));
    return QObject::qt_metacast(_clname);
}

int Communicate::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 5)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 5;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 5)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 5;
    }
    return _id;
}

// SIGNAL 0
void Communicate::rstResult(int _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void Communicate::Login(QJsonObject _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}

// SIGNAL 2
void Communicate::receiveSingleQuestion(QJsonObject _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 2, _a);
}

// SIGNAL 3
void Communicate::Rank(QJsonObject _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 3, _a);
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE

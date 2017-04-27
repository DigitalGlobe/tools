/****************************************************************************
** Meta object code from reading C++ file 'runnermodel.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.8.0)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../../../include/qxrunner/runnermodel.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'runnermodel.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.8.0. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_QxRunner__RunnerModel_t {
    QByteArrayData data[19];
    char stringdata0[294];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_QxRunner__RunnerModel_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_QxRunner__RunnerModel_t qt_meta_stringdata_QxRunner__RunnerModel = {
    {
QT_MOC_LITERAL(0, 0, 21), // "QxRunner::RunnerModel"
QT_MOC_LITERAL(1, 22, 11), // "itemStarted"
QT_MOC_LITERAL(2, 34, 0), // ""
QT_MOC_LITERAL(3, 35, 5), // "index"
QT_MOC_LITERAL(4, 41, 13), // "itemCompleted"
QT_MOC_LITERAL(5, 55, 17), // "allItemsCompleted"
QT_MOC_LITERAL(6, 73, 15), // "numTotalChanged"
QT_MOC_LITERAL(7, 89, 8), // "numItems"
QT_MOC_LITERAL(8, 98, 18), // "numSelectedChanged"
QT_MOC_LITERAL(9, 117, 17), // "numStartedChanged"
QT_MOC_LITERAL(10, 135, 19), // "numCompletedChanged"
QT_MOC_LITERAL(11, 155, 17), // "numSuccessChanged"
QT_MOC_LITERAL(12, 173, 15), // "numInfosChanged"
QT_MOC_LITERAL(13, 189, 18), // "numWarningsChanged"
QT_MOC_LITERAL(14, 208, 16), // "numErrorsChanged"
QT_MOC_LITERAL(15, 225, 16), // "numFatalsChanged"
QT_MOC_LITERAL(16, 242, 20), // "numExceptionsChanged"
QT_MOC_LITERAL(17, 263, 16), // "setMinimalUpdate"
QT_MOC_LITERAL(18, 280, 13) // "minimalUpdate"

    },
    "QxRunner::RunnerModel\0itemStarted\0\0"
    "index\0itemCompleted\0allItemsCompleted\0"
    "numTotalChanged\0numItems\0numSelectedChanged\0"
    "numStartedChanged\0numCompletedChanged\0"
    "numSuccessChanged\0numInfosChanged\0"
    "numWarningsChanged\0numErrorsChanged\0"
    "numFatalsChanged\0numExceptionsChanged\0"
    "setMinimalUpdate\0minimalUpdate"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_QxRunner__RunnerModel[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
      14,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
      13,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    1,   84,    2, 0x06 /* Public */,
       4,    1,   87,    2, 0x06 /* Public */,
       5,    0,   90,    2, 0x06 /* Public */,
       6,    1,   91,    2, 0x06 /* Public */,
       8,    1,   94,    2, 0x06 /* Public */,
       9,    1,   97,    2, 0x06 /* Public */,
      10,    1,  100,    2, 0x06 /* Public */,
      11,    1,  103,    2, 0x06 /* Public */,
      12,    1,  106,    2, 0x06 /* Public */,
      13,    1,  109,    2, 0x06 /* Public */,
      14,    1,  112,    2, 0x06 /* Public */,
      15,    1,  115,    2, 0x06 /* Public */,
      16,    1,  118,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
      17,    1,  121,    2, 0x0a /* Public */,

 // signals: parameters
    QMetaType::Void, QMetaType::QModelIndex,    3,
    QMetaType::Void, QMetaType::QModelIndex,    3,
    QMetaType::Void,
    QMetaType::Void, QMetaType::Int,    7,
    QMetaType::Void, QMetaType::Int,    7,
    QMetaType::Void, QMetaType::Int,    7,
    QMetaType::Void, QMetaType::Int,    7,
    QMetaType::Void, QMetaType::Int,    7,
    QMetaType::Void, QMetaType::Int,    7,
    QMetaType::Void, QMetaType::Int,    7,
    QMetaType::Void, QMetaType::Int,    7,
    QMetaType::Void, QMetaType::Int,    7,
    QMetaType::Void, QMetaType::Int,    7,

 // slots: parameters
    QMetaType::Void, QMetaType::Bool,   18,

       0        // eod
};

void QxRunner::RunnerModel::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        RunnerModel *_t = static_cast<RunnerModel *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->itemStarted((*reinterpret_cast< const QModelIndex(*)>(_a[1]))); break;
        case 1: _t->itemCompleted((*reinterpret_cast< const QModelIndex(*)>(_a[1]))); break;
        case 2: _t->allItemsCompleted(); break;
        case 3: _t->numTotalChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 4: _t->numSelectedChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 5: _t->numStartedChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 6: _t->numCompletedChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 7: _t->numSuccessChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 8: _t->numInfosChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 9: _t->numWarningsChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 10: _t->numErrorsChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 11: _t->numFatalsChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 12: _t->numExceptionsChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 13: _t->setMinimalUpdate((*reinterpret_cast< bool(*)>(_a[1]))); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        void **func = reinterpret_cast<void **>(_a[1]);
        {
            typedef void (RunnerModel::*_t)(const QModelIndex & ) const;
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&RunnerModel::itemStarted)) {
                *result = 0;
                return;
            }
        }
        {
            typedef void (RunnerModel::*_t)(const QModelIndex & ) const;
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&RunnerModel::itemCompleted)) {
                *result = 1;
                return;
            }
        }
        {
            typedef void (RunnerModel::*_t)() const;
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&RunnerModel::allItemsCompleted)) {
                *result = 2;
                return;
            }
        }
        {
            typedef void (RunnerModel::*_t)(int ) const;
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&RunnerModel::numTotalChanged)) {
                *result = 3;
                return;
            }
        }
        {
            typedef void (RunnerModel::*_t)(int ) const;
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&RunnerModel::numSelectedChanged)) {
                *result = 4;
                return;
            }
        }
        {
            typedef void (RunnerModel::*_t)(int ) const;
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&RunnerModel::numStartedChanged)) {
                *result = 5;
                return;
            }
        }
        {
            typedef void (RunnerModel::*_t)(int ) const;
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&RunnerModel::numCompletedChanged)) {
                *result = 6;
                return;
            }
        }
        {
            typedef void (RunnerModel::*_t)(int ) const;
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&RunnerModel::numSuccessChanged)) {
                *result = 7;
                return;
            }
        }
        {
            typedef void (RunnerModel::*_t)(int ) const;
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&RunnerModel::numInfosChanged)) {
                *result = 8;
                return;
            }
        }
        {
            typedef void (RunnerModel::*_t)(int ) const;
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&RunnerModel::numWarningsChanged)) {
                *result = 9;
                return;
            }
        }
        {
            typedef void (RunnerModel::*_t)(int ) const;
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&RunnerModel::numErrorsChanged)) {
                *result = 10;
                return;
            }
        }
        {
            typedef void (RunnerModel::*_t)(int ) const;
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&RunnerModel::numFatalsChanged)) {
                *result = 11;
                return;
            }
        }
        {
            typedef void (RunnerModel::*_t)(int ) const;
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&RunnerModel::numExceptionsChanged)) {
                *result = 12;
                return;
            }
        }
    }
}

const QMetaObject QxRunner::RunnerModel::staticMetaObject = {
    { &QAbstractItemModel::staticMetaObject, qt_meta_stringdata_QxRunner__RunnerModel.data,
      qt_meta_data_QxRunner__RunnerModel,  qt_static_metacall, Q_NULLPTR, Q_NULLPTR}
};


const QMetaObject *QxRunner::RunnerModel::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *QxRunner::RunnerModel::qt_metacast(const char *_clname)
{
    if (!_clname) return Q_NULLPTR;
    if (!strcmp(_clname, qt_meta_stringdata_QxRunner__RunnerModel.stringdata0))
        return static_cast<void*>(const_cast< RunnerModel*>(this));
    return QAbstractItemModel::qt_metacast(_clname);
}

int QxRunner::RunnerModel::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QAbstractItemModel::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 14)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 14;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 14)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 14;
    }
    return _id;
}

// SIGNAL 0
void QxRunner::RunnerModel::itemStarted(const QModelIndex & _t1)const
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(const_cast< QxRunner::RunnerModel *>(this), &staticMetaObject, 0, _a);
}

// SIGNAL 1
void QxRunner::RunnerModel::itemCompleted(const QModelIndex & _t1)const
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(const_cast< QxRunner::RunnerModel *>(this), &staticMetaObject, 1, _a);
}

// SIGNAL 2
void QxRunner::RunnerModel::allItemsCompleted()const
{
    QMetaObject::activate(const_cast< QxRunner::RunnerModel *>(this), &staticMetaObject, 2, Q_NULLPTR);
}

// SIGNAL 3
void QxRunner::RunnerModel::numTotalChanged(int _t1)const
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(const_cast< QxRunner::RunnerModel *>(this), &staticMetaObject, 3, _a);
}

// SIGNAL 4
void QxRunner::RunnerModel::numSelectedChanged(int _t1)const
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(const_cast< QxRunner::RunnerModel *>(this), &staticMetaObject, 4, _a);
}

// SIGNAL 5
void QxRunner::RunnerModel::numStartedChanged(int _t1)const
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(const_cast< QxRunner::RunnerModel *>(this), &staticMetaObject, 5, _a);
}

// SIGNAL 6
void QxRunner::RunnerModel::numCompletedChanged(int _t1)const
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(const_cast< QxRunner::RunnerModel *>(this), &staticMetaObject, 6, _a);
}

// SIGNAL 7
void QxRunner::RunnerModel::numSuccessChanged(int _t1)const
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(const_cast< QxRunner::RunnerModel *>(this), &staticMetaObject, 7, _a);
}

// SIGNAL 8
void QxRunner::RunnerModel::numInfosChanged(int _t1)const
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(const_cast< QxRunner::RunnerModel *>(this), &staticMetaObject, 8, _a);
}

// SIGNAL 9
void QxRunner::RunnerModel::numWarningsChanged(int _t1)const
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(const_cast< QxRunner::RunnerModel *>(this), &staticMetaObject, 9, _a);
}

// SIGNAL 10
void QxRunner::RunnerModel::numErrorsChanged(int _t1)const
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(const_cast< QxRunner::RunnerModel *>(this), &staticMetaObject, 10, _a);
}

// SIGNAL 11
void QxRunner::RunnerModel::numFatalsChanged(int _t1)const
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(const_cast< QxRunner::RunnerModel *>(this), &staticMetaObject, 11, _a);
}

// SIGNAL 12
void QxRunner::RunnerModel::numExceptionsChanged(int _t1)const
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(const_cast< QxRunner::RunnerModel *>(this), &staticMetaObject, 12, _a);
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE

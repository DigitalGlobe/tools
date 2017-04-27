/****************************************************************************
** Meta object code from reading C++ file 'runnerviewcontroller.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.8.0)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../../../include/qxrunner/runnerviewcontroller.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'runnerviewcontroller.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.8.0. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_QxRunner__RunnerViewController_t {
    QByteArrayData data[6];
    char stringdata0[76];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_QxRunner__RunnerViewController_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_QxRunner__RunnerViewController_t qt_meta_stringdata_QxRunner__RunnerViewController = {
    {
QT_MOC_LITERAL(0, 0, 30), // "QxRunner::RunnerViewController"
QT_MOC_LITERAL(1, 31, 9), // "selectAll"
QT_MOC_LITERAL(2, 41, 0), // ""
QT_MOC_LITERAL(3, 42, 11), // "unselectAll"
QT_MOC_LITERAL(4, 54, 9), // "expandAll"
QT_MOC_LITERAL(5, 64, 11) // "collapseAll"

    },
    "QxRunner::RunnerViewController\0selectAll\0"
    "\0unselectAll\0expandAll\0collapseAll"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_QxRunner__RunnerViewController[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       4,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: name, argc, parameters, tag, flags
       1,    0,   34,    2, 0x0a /* Public */,
       3,    0,   35,    2, 0x0a /* Public */,
       4,    0,   36,    2, 0x0a /* Public */,
       5,    0,   37,    2, 0x0a /* Public */,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,

       0        // eod
};

void QxRunner::RunnerViewController::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        RunnerViewController *_t = static_cast<RunnerViewController *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->selectAll(); break;
        case 1: _t->unselectAll(); break;
        case 2: _t->expandAll(); break;
        case 3: _t->collapseAll(); break;
        default: ;
        }
    }
    Q_UNUSED(_a);
}

const QMetaObject QxRunner::RunnerViewController::staticMetaObject = {
    { &QObject::staticMetaObject, qt_meta_stringdata_QxRunner__RunnerViewController.data,
      qt_meta_data_QxRunner__RunnerViewController,  qt_static_metacall, Q_NULLPTR, Q_NULLPTR}
};


const QMetaObject *QxRunner::RunnerViewController::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *QxRunner::RunnerViewController::qt_metacast(const char *_clname)
{
    if (!_clname) return Q_NULLPTR;
    if (!strcmp(_clname, qt_meta_stringdata_QxRunner__RunnerViewController.stringdata0))
        return static_cast<void*>(const_cast< RunnerViewController*>(this));
    if (!strcmp(_clname, "ViewControllerCommon"))
        return static_cast< ViewControllerCommon*>(const_cast< RunnerViewController*>(this));
    return QObject::qt_metacast(_clname);
}

int QxRunner::RunnerViewController::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 4)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 4;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 4)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 4;
    }
    return _id;
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE

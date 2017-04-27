/****************************************************************************
** Meta object code from reading C++ file 'testquazip.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.7.0)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../testquazip.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'testquazip.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.7.0. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
struct qt_meta_stringdata_TestQuaZip_t {
    QByteArrayData data[15];
    char stringdata0[209];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_TestQuaZip_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_TestQuaZip_t qt_meta_stringdata_TestQuaZip = {
    {
QT_MOC_LITERAL(0, 0, 10), // "TestQuaZip"
QT_MOC_LITERAL(1, 11, 16), // "getFileList_data"
QT_MOC_LITERAL(2, 28, 0), // ""
QT_MOC_LITERAL(3, 29, 11), // "getFileList"
QT_MOC_LITERAL(4, 41, 8), // "add_data"
QT_MOC_LITERAL(5, 50, 3), // "add"
QT_MOC_LITERAL(6, 54, 21), // "setFileNameCodec_data"
QT_MOC_LITERAL(7, 76, 16), // "setFileNameCodec"
QT_MOC_LITERAL(8, 93, 31), // "setDataDescriptorWritingEnabled"
QT_MOC_LITERAL(9, 125, 16), // "testQIODeviceAPI"
QT_MOC_LITERAL(10, 142, 10), // "setZipName"
QT_MOC_LITERAL(11, 153, 11), // "setIoDevice"
QT_MOC_LITERAL(12, 165, 15), // "setCommentCodec"
QT_MOC_LITERAL(13, 181, 12), // "setAutoClose"
QT_MOC_LITERAL(14, 194, 14) // "testSequential"

    },
    "TestQuaZip\0getFileList_data\0\0getFileList\0"
    "add_data\0add\0setFileNameCodec_data\0"
    "setFileNameCodec\0setDataDescriptorWritingEnabled\0"
    "testQIODeviceAPI\0setZipName\0setIoDevice\0"
    "setCommentCodec\0setAutoClose\0"
    "testSequential"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_TestQuaZip[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
      13,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: name, argc, parameters, tag, flags
       1,    0,   79,    2, 0x08 /* Private */,
       3,    0,   80,    2, 0x08 /* Private */,
       4,    0,   81,    2, 0x08 /* Private */,
       5,    0,   82,    2, 0x08 /* Private */,
       6,    0,   83,    2, 0x08 /* Private */,
       7,    0,   84,    2, 0x08 /* Private */,
       8,    0,   85,    2, 0x08 /* Private */,
       9,    0,   86,    2, 0x08 /* Private */,
      10,    0,   87,    2, 0x08 /* Private */,
      11,    0,   88,    2, 0x08 /* Private */,
      12,    0,   89,    2, 0x08 /* Private */,
      13,    0,   90,    2, 0x08 /* Private */,
      14,    0,   91,    2, 0x08 /* Private */,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,

       0        // eod
};

void TestQuaZip::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        TestQuaZip *_t = static_cast<TestQuaZip *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->getFileList_data(); break;
        case 1: _t->getFileList(); break;
        case 2: _t->add_data(); break;
        case 3: _t->add(); break;
        case 4: _t->setFileNameCodec_data(); break;
        case 5: _t->setFileNameCodec(); break;
        case 6: _t->setDataDescriptorWritingEnabled(); break;
        case 7: _t->testQIODeviceAPI(); break;
        case 8: _t->setZipName(); break;
        case 9: _t->setIoDevice(); break;
        case 10: _t->setCommentCodec(); break;
        case 11: _t->setAutoClose(); break;
        case 12: _t->testSequential(); break;
        default: ;
        }
    }
    Q_UNUSED(_a);
}

const QMetaObject TestQuaZip::staticMetaObject = {
    { &QObject::staticMetaObject, qt_meta_stringdata_TestQuaZip.data,
      qt_meta_data_TestQuaZip,  qt_static_metacall, Q_NULLPTR, Q_NULLPTR}
};


const QMetaObject *TestQuaZip::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *TestQuaZip::qt_metacast(const char *_clname)
{
    if (!_clname) return Q_NULLPTR;
    if (!strcmp(_clname, qt_meta_stringdata_TestQuaZip.stringdata0))
        return static_cast<void*>(const_cast< TestQuaZip*>(this));
    return QObject::qt_metacast(_clname);
}

int TestQuaZip::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 13)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 13;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 13)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 13;
    }
    return _id;
}
QT_END_MOC_NAMESPACE

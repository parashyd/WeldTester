/****************************************************************************
** Meta object code from reading C++ file 'testdetails.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.15.13)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include <memory>
#include "include/testdetails.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'testdetails.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.15.13. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_TestDetails_t {
    QByteArrayData data[19];
    char stringdata0[235];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_TestDetails_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_TestDetails_t qt_meta_stringdata_TestDetails = {
    {
QT_MOC_LITERAL(0, 0, 11), // "TestDetails"
QT_MOC_LITERAL(1, 12, 14), // "requestPreview"
QT_MOC_LITERAL(2, 27, 0), // ""
QT_MOC_LITERAL(3, 28, 12), // "enterPressed"
QT_MOC_LITERAL(4, 41, 10), // "QLineEdit*"
QT_MOC_LITERAL(5, 52, 8), // "lineEdit"
QT_MOC_LITERAL(6, 61, 12), // "ctrlTPressed"
QT_MOC_LITERAL(7, 74, 21), // "handleMultiPressAlpha"
QT_MOC_LITERAL(8, 96, 3), // "key"
QT_MOC_LITERAL(9, 100, 13), // "navigateFocus"
QT_MOC_LITERAL(10, 114, 9), // "direction"
QT_MOC_LITERAL(11, 124, 19), // "adjustComboBoxIndex"
QT_MOC_LITERAL(12, 144, 5), // "delta"
QT_MOC_LITERAL(13, 150, 20), // "handleBackspaceInput"
QT_MOC_LITERAL(14, 171, 14), // "saveDataToFile"
QT_MOC_LITERAL(15, 186, 16), // "loadDataFromFile"
QT_MOC_LITERAL(16, 203, 15), // "setLogicalFocus"
QT_MOC_LITERAL(17, 219, 8), // "QWidget*"
QT_MOC_LITERAL(18, 228, 6) // "widget"

    },
    "TestDetails\0requestPreview\0\0enterPressed\0"
    "QLineEdit*\0lineEdit\0ctrlTPressed\0"
    "handleMultiPressAlpha\0key\0navigateFocus\0"
    "direction\0adjustComboBoxIndex\0delta\0"
    "handleBackspaceInput\0saveDataToFile\0"
    "loadDataFromFile\0setLogicalFocus\0"
    "QWidget*\0widget"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_TestDetails[] = {

 // content:
       8,       // revision
       0,       // classname
       0,    0, // classinfo
      10,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       3,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    0,   64,    2, 0x06 /* Public */,
       3,    1,   65,    2, 0x06 /* Public */,
       6,    1,   68,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
       7,    2,   71,    2, 0x08 /* Private */,
       9,    1,   76,    2, 0x08 /* Private */,
      11,    1,   79,    2, 0x08 /* Private */,
      13,    0,   82,    2, 0x08 /* Private */,
      14,    0,   83,    2, 0x08 /* Private */,
      15,    0,   84,    2, 0x08 /* Private */,
      16,    1,   85,    2, 0x08 /* Private */,

 // signals: parameters
    QMetaType::Void,
    QMetaType::Void, 0x80000000 | 4,    5,
    QMetaType::Void, 0x80000000 | 4,    5,

 // slots: parameters
    QMetaType::Void, QMetaType::UChar, 0x80000000 | 4,    8,    5,
    QMetaType::Void, QMetaType::Int,   10,
    QMetaType::Void, QMetaType::Int,   12,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, 0x80000000 | 17,   18,

       0        // eod
};

void TestDetails::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<TestDetails *>(_o);
        (void)_t;
        switch (_id) {
        case 0: _t->requestPreview(); break;
        case 1: _t->enterPressed((*reinterpret_cast< QLineEdit*(*)>(_a[1]))); break;
        case 2: _t->ctrlTPressed((*reinterpret_cast< QLineEdit*(*)>(_a[1]))); break;
        case 3: _t->handleMultiPressAlpha((*reinterpret_cast< quint8(*)>(_a[1])),(*reinterpret_cast< QLineEdit*(*)>(_a[2]))); break;
        case 4: _t->navigateFocus((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 5: _t->adjustComboBoxIndex((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 6: _t->handleBackspaceInput(); break;
        case 7: _t->saveDataToFile(); break;
        case 8: _t->loadDataFromFile(); break;
        case 9: _t->setLogicalFocus((*reinterpret_cast< QWidget*(*)>(_a[1]))); break;
        default: ;
        }
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        switch (_id) {
        default: *reinterpret_cast<int*>(_a[0]) = -1; break;
        case 1:
            switch (*reinterpret_cast<int*>(_a[1])) {
            default: *reinterpret_cast<int*>(_a[0]) = -1; break;
            case 0:
                *reinterpret_cast<int*>(_a[0]) = qRegisterMetaType< QLineEdit* >(); break;
            }
            break;
        case 2:
            switch (*reinterpret_cast<int*>(_a[1])) {
            default: *reinterpret_cast<int*>(_a[0]) = -1; break;
            case 0:
                *reinterpret_cast<int*>(_a[0]) = qRegisterMetaType< QLineEdit* >(); break;
            }
            break;
        case 3:
            switch (*reinterpret_cast<int*>(_a[1])) {
            default: *reinterpret_cast<int*>(_a[0]) = -1; break;
            case 1:
                *reinterpret_cast<int*>(_a[0]) = qRegisterMetaType< QLineEdit* >(); break;
            }
            break;
        case 9:
            switch (*reinterpret_cast<int*>(_a[1])) {
            default: *reinterpret_cast<int*>(_a[0]) = -1; break;
            case 0:
                *reinterpret_cast<int*>(_a[0]) = qRegisterMetaType< QWidget* >(); break;
            }
            break;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        {
            using _t = void (TestDetails::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&TestDetails::requestPreview)) {
                *result = 0;
                return;
            }
        }
        {
            using _t = void (TestDetails::*)(QLineEdit * );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&TestDetails::enterPressed)) {
                *result = 1;
                return;
            }
        }
        {
            using _t = void (TestDetails::*)(QLineEdit * );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&TestDetails::ctrlTPressed)) {
                *result = 2;
                return;
            }
        }
    }
}

QT_INIT_METAOBJECT const QMetaObject TestDetails::staticMetaObject = { {
    QMetaObject::SuperData::link<QDialog::staticMetaObject>(),
    qt_meta_stringdata_TestDetails.data,
    qt_meta_data_TestDetails,
    qt_static_metacall,
    nullptr,
    nullptr
} };


const QMetaObject *TestDetails::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *TestDetails::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_TestDetails.stringdata0))
        return static_cast<void*>(this);
    return QDialog::qt_metacast(_clname);
}

int TestDetails::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QDialog::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 10)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 10;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 10)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 10;
    }
    return _id;
}

// SIGNAL 0
void TestDetails::requestPreview()
{
    QMetaObject::activate(this, &staticMetaObject, 0, nullptr);
}

// SIGNAL 1
void TestDetails::enterPressed(QLineEdit * _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}

// SIGNAL 2
void TestDetails::ctrlTPressed(QLineEdit * _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 2, _a);
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE

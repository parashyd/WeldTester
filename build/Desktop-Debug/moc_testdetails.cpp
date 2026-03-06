/****************************************************************************
** Meta object code from reading C++ file 'testdetails.h'
**
** Created by: The Qt Meta Object Compiler version 69 (Qt 6.9.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../include/testdetails.h"
#include <QtGui/qtextcursor.h>
#include <QtCore/qmetatype.h>

#include <QtCore/qtmochelpers.h>

#include <memory>


#include <QtCore/qxptype_traits.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'testdetails.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 69
#error "This file was generated using the moc from 6.9.1. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

#ifndef Q_CONSTINIT
#define Q_CONSTINIT
#endif

QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
QT_WARNING_DISABLE_GCC("-Wuseless-cast")
namespace {
struct qt_meta_tag_ZN11TestDetailsE_t {};
} // unnamed namespace

template <> constexpr inline auto TestDetails::qt_create_metaobjectdata<qt_meta_tag_ZN11TestDetailsE_t>()
{
    namespace QMC = QtMocConstants;
    QtMocHelpers::StringRefStorage qt_stringData {
        "TestDetails",
        "requestPreview",
        "",
        "enterPressed",
        "QLineEdit*",
        "lineEdit",
        "ctrlTPressed",
        "handleMultiPressAlpha",
        "key",
        "navigateFocus",
        "direction",
        "adjustComboBoxIndex",
        "delta",
        "handleBackspaceInput",
        "saveDataToFile",
        "loadDataFromFile"
    };

    QtMocHelpers::UintData qt_methods {
        // Signal 'requestPreview'
        QtMocHelpers::SignalData<void()>(1, 2, QMC::AccessPublic, QMetaType::Void),
        // Signal 'enterPressed'
        QtMocHelpers::SignalData<void(QLineEdit *)>(3, 2, QMC::AccessPublic, QMetaType::Void, {{
            { 0x80000000 | 4, 5 },
        }}),
        // Signal 'ctrlTPressed'
        QtMocHelpers::SignalData<void(QLineEdit *)>(6, 2, QMC::AccessPublic, QMetaType::Void, {{
            { 0x80000000 | 4, 5 },
        }}),
        // Slot 'handleMultiPressAlpha'
        QtMocHelpers::SlotData<void(quint8, QLineEdit *)>(7, 2, QMC::AccessPrivate, QMetaType::Void, {{
            { QMetaType::UChar, 8 }, { 0x80000000 | 4, 5 },
        }}),
        // Slot 'navigateFocus'
        QtMocHelpers::SlotData<void(int)>(9, 2, QMC::AccessPrivate, QMetaType::Void, {{
            { QMetaType::Int, 10 },
        }}),
        // Slot 'adjustComboBoxIndex'
        QtMocHelpers::SlotData<void(int)>(11, 2, QMC::AccessPrivate, QMetaType::Void, {{
            { QMetaType::Int, 12 },
        }}),
        // Slot 'handleBackspaceInput'
        QtMocHelpers::SlotData<void()>(13, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'saveDataToFile'
        QtMocHelpers::SlotData<void()>(14, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'loadDataFromFile'
        QtMocHelpers::SlotData<void()>(15, 2, QMC::AccessPrivate, QMetaType::Void),
    };
    QtMocHelpers::UintData qt_properties {
    };
    QtMocHelpers::UintData qt_enums {
    };
    return QtMocHelpers::metaObjectData<TestDetails, qt_meta_tag_ZN11TestDetailsE_t>(QMC::MetaObjectFlag{}, qt_stringData,
            qt_methods, qt_properties, qt_enums);
}
Q_CONSTINIT const QMetaObject TestDetails::staticMetaObject = { {
    QMetaObject::SuperData::link<QDialog::staticMetaObject>(),
    qt_staticMetaObjectStaticContent<qt_meta_tag_ZN11TestDetailsE_t>.stringdata,
    qt_staticMetaObjectStaticContent<qt_meta_tag_ZN11TestDetailsE_t>.data,
    qt_static_metacall,
    nullptr,
    qt_staticMetaObjectRelocatingContent<qt_meta_tag_ZN11TestDetailsE_t>.metaTypes,
    nullptr
} };

void TestDetails::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    auto *_t = static_cast<TestDetails *>(_o);
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: _t->requestPreview(); break;
        case 1: _t->enterPressed((*reinterpret_cast< std::add_pointer_t<QLineEdit*>>(_a[1]))); break;
        case 2: _t->ctrlTPressed((*reinterpret_cast< std::add_pointer_t<QLineEdit*>>(_a[1]))); break;
        case 3: _t->handleMultiPressAlpha((*reinterpret_cast< std::add_pointer_t<quint8>>(_a[1])),(*reinterpret_cast< std::add_pointer_t<QLineEdit*>>(_a[2]))); break;
        case 4: _t->navigateFocus((*reinterpret_cast< std::add_pointer_t<int>>(_a[1]))); break;
        case 5: _t->adjustComboBoxIndex((*reinterpret_cast< std::add_pointer_t<int>>(_a[1]))); break;
        case 6: _t->handleBackspaceInput(); break;
        case 7: _t->saveDataToFile(); break;
        case 8: _t->loadDataFromFile(); break;
        default: ;
        }
    }
    if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        switch (_id) {
        default: *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType(); break;
        case 1:
            switch (*reinterpret_cast<int*>(_a[1])) {
            default: *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType(); break;
            case 0:
                *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType::fromType< QLineEdit* >(); break;
            }
            break;
        case 2:
            switch (*reinterpret_cast<int*>(_a[1])) {
            default: *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType(); break;
            case 0:
                *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType::fromType< QLineEdit* >(); break;
            }
            break;
        case 3:
            switch (*reinterpret_cast<int*>(_a[1])) {
            default: *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType(); break;
            case 1:
                *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType::fromType< QLineEdit* >(); break;
            }
            break;
        }
    }
    if (_c == QMetaObject::IndexOfMethod) {
        if (QtMocHelpers::indexOfMethod<void (TestDetails::*)()>(_a, &TestDetails::requestPreview, 0))
            return;
        if (QtMocHelpers::indexOfMethod<void (TestDetails::*)(QLineEdit * )>(_a, &TestDetails::enterPressed, 1))
            return;
        if (QtMocHelpers::indexOfMethod<void (TestDetails::*)(QLineEdit * )>(_a, &TestDetails::ctrlTPressed, 2))
            return;
    }
}

const QMetaObject *TestDetails::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *TestDetails::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_staticMetaObjectStaticContent<qt_meta_tag_ZN11TestDetailsE_t>.strings))
        return static_cast<void*>(this);
    return QDialog::qt_metacast(_clname);
}

int TestDetails::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QDialog::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 9)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 9;
    }
    if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 9)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 9;
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
    QMetaObject::activate<void>(this, &staticMetaObject, 1, nullptr, _t1);
}

// SIGNAL 2
void TestDetails::ctrlTPressed(QLineEdit * _t1)
{
    QMetaObject::activate<void>(this, &staticMetaObject, 2, nullptr, _t1);
}
QT_WARNING_POP

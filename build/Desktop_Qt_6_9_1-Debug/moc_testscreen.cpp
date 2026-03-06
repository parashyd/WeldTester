/****************************************************************************
** Meta object code from reading C++ file 'testscreen.h'
**
** Created by: The Qt Meta Object Compiler version 69 (Qt 6.9.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../include/testscreen.h"
#include <QtGui/qtextcursor.h>
#include <QtGui/qscreen.h>
#include <QtCore/qmetatype.h>

#include <QtCore/qtmochelpers.h>

#include <memory>


#include <QtCore/qxptype_traits.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'testscreen.h' doesn't include <QObject>."
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
struct qt_meta_tag_ZN10TestScreenE_t {};
} // unnamed namespace

template <> constexpr inline auto TestScreen::qt_create_metaobjectdata<qt_meta_tag_ZN10TestScreenE_t>()
{
    namespace QMC = QtMocConstants;
    QtMocHelpers::StringRefStorage qt_stringData {
        "TestScreen",
        "updateConfigFile",
        "",
        "filePath",
        "ConfigEntry",
        "updatedConfig",
        "setupPlotAppearance",
        "updateGridInterval",
        "autoRunConfig",
        "saveTo_entry",
        "handleDigitInput",
        "digit",
        "navigateFocusVertical",
        "direction",
        "forceStartFromGain",
        "setInputFieldsEnabled",
        "enabled",
        "adjustCurrentLineEdit",
        "delta",
        "updateGraphWithData",
        "onApplyGainClicked",
        "CloseDAC",
        "SaveDACPar",
        "UserDACPoints",
        "DrawDACCurve",
        "onNewConnection",
        "onSocketReadyRead",
        "loadSavedConfig",
        "saveConfigToFile",
        "setupValidators",
        "BatteryIndication",
        "updateBatteryLevel",
        "percent",
        "handleOtherScreenKeys",
        "uint8_t",
        "key",
        "handleBackspaceInput",
        "normalizeKeyForNumeric",
        "paintAudioIndicator",
        "QPainter&",
        "p",
        "eventFilter",
        "obj",
        "QEvent*",
        "event",
        "showBlankScreen",
        "prepareVelocityInput",
        "prepareCalsetInput",
        "adjustAudioLevel",
        "handleSaveFlow",
        "handleFreezeLogic"
    };

    QtMocHelpers::UintData qt_methods {
        // Slot 'updateConfigFile'
        QtMocHelpers::SlotData<bool(const QString &, const ConfigEntry &)>(1, 2, QMC::AccessPrivate, QMetaType::Bool, {{
            { QMetaType::QString, 3 }, { 0x80000000 | 4, 5 },
        }}),
        // Slot 'setupPlotAppearance'
        QtMocHelpers::SlotData<void()>(6, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'updateGridInterval'
        QtMocHelpers::SlotData<void()>(7, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'autoRunConfig'
        QtMocHelpers::SlotData<void()>(8, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'saveTo_entry'
        QtMocHelpers::SlotData<void()>(9, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'handleDigitInput'
        QtMocHelpers::SlotData<void(int)>(10, 2, QMC::AccessPrivate, QMetaType::Void, {{
            { QMetaType::Int, 11 },
        }}),
        // Slot 'navigateFocusVertical'
        QtMocHelpers::SlotData<void(int, bool)>(12, 2, QMC::AccessPrivate, QMetaType::Void, {{
            { QMetaType::Int, 13 }, { QMetaType::Bool, 14 },
        }}),
        // Slot 'setInputFieldsEnabled'
        QtMocHelpers::SlotData<void(bool)>(15, 2, QMC::AccessPrivate, QMetaType::Void, {{
            { QMetaType::Bool, 16 },
        }}),
        // Slot 'adjustCurrentLineEdit'
        QtMocHelpers::SlotData<void(int)>(17, 2, QMC::AccessPrivate, QMetaType::Void, {{
            { QMetaType::Int, 18 },
        }}),
        // Slot 'updateGraphWithData'
        QtMocHelpers::SlotData<void()>(19, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'onApplyGainClicked'
        QtMocHelpers::SlotData<void()>(20, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'CloseDAC'
        QtMocHelpers::SlotData<void()>(21, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'SaveDACPar'
        QtMocHelpers::SlotData<void()>(22, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'UserDACPoints'
        QtMocHelpers::SlotData<void()>(23, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'DrawDACCurve'
        QtMocHelpers::SlotData<void()>(24, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'onNewConnection'
        QtMocHelpers::SlotData<void()>(25, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'onSocketReadyRead'
        QtMocHelpers::SlotData<void()>(26, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'loadSavedConfig'
        QtMocHelpers::SlotData<void()>(27, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'saveConfigToFile'
        QtMocHelpers::SlotData<void()>(28, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'setupValidators'
        QtMocHelpers::SlotData<void()>(29, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'BatteryIndication'
        QtMocHelpers::SlotData<void()>(30, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'updateBatteryLevel'
        QtMocHelpers::SlotData<void(int)>(31, 2, QMC::AccessPrivate, QMetaType::Void, {{
            { QMetaType::Int, 32 },
        }}),
        // Slot 'handleOtherScreenKeys'
        QtMocHelpers::SlotData<void(uint8_t)>(33, 2, QMC::AccessPrivate, QMetaType::Void, {{
            { 0x80000000 | 34, 35 },
        }}),
        // Slot 'handleBackspaceInput'
        QtMocHelpers::SlotData<void()>(36, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'normalizeKeyForNumeric'
        QtMocHelpers::SlotData<int(uint8_t)>(37, 2, QMC::AccessPrivate, QMetaType::Int, {{
            { 0x80000000 | 34, 35 },
        }}),
        // Slot 'paintAudioIndicator'
        QtMocHelpers::SlotData<void(QPainter &)>(38, 2, QMC::AccessPrivate, QMetaType::Void, {{
            { 0x80000000 | 39, 40 },
        }}),
        // Slot 'eventFilter'
        QtMocHelpers::SlotData<bool(QObject *, QEvent *)>(41, 2, QMC::AccessPrivate, QMetaType::Bool, {{
            { QMetaType::QObjectStar, 42 }, { 0x80000000 | 43, 44 },
        }}),
        // Slot 'showBlankScreen'
        QtMocHelpers::SlotData<void()>(45, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'prepareVelocityInput'
        QtMocHelpers::SlotData<void()>(46, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'prepareCalsetInput'
        QtMocHelpers::SlotData<void()>(47, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'adjustAudioLevel'
        QtMocHelpers::SlotData<void()>(48, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'handleSaveFlow'
        QtMocHelpers::SlotData<void()>(49, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'handleFreezeLogic'
        QtMocHelpers::SlotData<void()>(50, 2, QMC::AccessPrivate, QMetaType::Void),
    };
    QtMocHelpers::UintData qt_properties {
    };
    QtMocHelpers::UintData qt_enums {
    };
    return QtMocHelpers::metaObjectData<TestScreen, qt_meta_tag_ZN10TestScreenE_t>(QMC::MetaObjectFlag{}, qt_stringData,
            qt_methods, qt_properties, qt_enums);
}
Q_CONSTINIT const QMetaObject TestScreen::staticMetaObject = { {
    QMetaObject::SuperData::link<QMainWindow::staticMetaObject>(),
    qt_staticMetaObjectStaticContent<qt_meta_tag_ZN10TestScreenE_t>.stringdata,
    qt_staticMetaObjectStaticContent<qt_meta_tag_ZN10TestScreenE_t>.data,
    qt_static_metacall,
    nullptr,
    qt_staticMetaObjectRelocatingContent<qt_meta_tag_ZN10TestScreenE_t>.metaTypes,
    nullptr
} };

void TestScreen::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    auto *_t = static_cast<TestScreen *>(_o);
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: { bool _r = _t->updateConfigFile((*reinterpret_cast< std::add_pointer_t<QString>>(_a[1])),(*reinterpret_cast< std::add_pointer_t<ConfigEntry>>(_a[2])));
            if (_a[0]) *reinterpret_cast< bool*>(_a[0]) = std::move(_r); }  break;
        case 1: _t->setupPlotAppearance(); break;
        case 2: _t->updateGridInterval(); break;
        case 3: _t->autoRunConfig(); break;
        case 4: _t->saveTo_entry(); break;
        case 5: _t->handleDigitInput((*reinterpret_cast< std::add_pointer_t<int>>(_a[1]))); break;
        case 6: _t->navigateFocusVertical((*reinterpret_cast< std::add_pointer_t<int>>(_a[1])),(*reinterpret_cast< std::add_pointer_t<bool>>(_a[2]))); break;
        case 7: _t->setInputFieldsEnabled((*reinterpret_cast< std::add_pointer_t<bool>>(_a[1]))); break;
        case 8: _t->adjustCurrentLineEdit((*reinterpret_cast< std::add_pointer_t<int>>(_a[1]))); break;
        case 9: _t->updateGraphWithData(); break;
        case 10: _t->onApplyGainClicked(); break;
        case 11: _t->CloseDAC(); break;
        case 12: _t->SaveDACPar(); break;
        case 13: _t->UserDACPoints(); break;
        case 14: _t->DrawDACCurve(); break;
        case 15: _t->onNewConnection(); break;
        case 16: _t->onSocketReadyRead(); break;
        case 17: _t->loadSavedConfig(); break;
        case 18: _t->saveConfigToFile(); break;
        case 19: _t->setupValidators(); break;
        case 20: _t->BatteryIndication(); break;
        case 21: _t->updateBatteryLevel((*reinterpret_cast< std::add_pointer_t<int>>(_a[1]))); break;
        case 22: _t->handleOtherScreenKeys((*reinterpret_cast< std::add_pointer_t<uint8_t>>(_a[1]))); break;
        case 23: _t->handleBackspaceInput(); break;
        case 24: { int _r = _t->normalizeKeyForNumeric((*reinterpret_cast< std::add_pointer_t<uint8_t>>(_a[1])));
            if (_a[0]) *reinterpret_cast< int*>(_a[0]) = std::move(_r); }  break;
        case 25: _t->paintAudioIndicator((*reinterpret_cast< std::add_pointer_t<QPainter&>>(_a[1]))); break;
        case 26: { bool _r = _t->eventFilter((*reinterpret_cast< std::add_pointer_t<QObject*>>(_a[1])),(*reinterpret_cast< std::add_pointer_t<QEvent*>>(_a[2])));
            if (_a[0]) *reinterpret_cast< bool*>(_a[0]) = std::move(_r); }  break;
        case 27: _t->showBlankScreen(); break;
        case 28: _t->prepareVelocityInput(); break;
        case 29: _t->prepareCalsetInput(); break;
        case 30: _t->adjustAudioLevel(); break;
        case 31: _t->handleSaveFlow(); break;
        case 32: _t->handleFreezeLogic(); break;
        default: ;
        }
    }
}

const QMetaObject *TestScreen::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *TestScreen::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_staticMetaObjectStaticContent<qt_meta_tag_ZN10TestScreenE_t>.strings))
        return static_cast<void*>(this);
    return QMainWindow::qt_metacast(_clname);
}

int TestScreen::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QMainWindow::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 33)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 33;
    }
    if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 33)
            *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType();
        _id -= 33;
    }
    return _id;
}
QT_WARNING_POP

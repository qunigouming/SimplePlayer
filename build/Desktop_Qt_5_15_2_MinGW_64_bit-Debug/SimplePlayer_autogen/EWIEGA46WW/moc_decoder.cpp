/****************************************************************************
** Meta object code from reading C++ file 'decoder.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.15.2)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include <memory>
#include "../../../../decoder.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'decoder.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.15.2. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_AudioDecoder_t {
    QByteArrayData data[7];
    char stringdata0[60];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_AudioDecoder_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_AudioDecoder_t qt_meta_stringdata_AudioDecoder = {
    {
QT_MOC_LITERAL(0, 0, 12), // "AudioDecoder"
QT_MOC_LITERAL(1, 13, 12), // "clockChanged"
QT_MOC_LITERAL(2, 26, 0), // ""
QT_MOC_LITERAL(3, 27, 5), // "clock"
QT_MOC_LITERAL(4, 33, 14), // "onStateChanged"
QT_MOC_LITERAL(5, 48, 5), // "State"
QT_MOC_LITERAL(6, 54, 5) // "state"

    },
    "AudioDecoder\0clockChanged\0\0clock\0"
    "onStateChanged\0State\0state"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_AudioDecoder[] = {

 // content:
       8,       // revision
       0,       // classname
       0,    0, // classinfo
       2,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       1,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    1,   24,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
       4,    1,   27,    2, 0x0a /* Public */,

 // signals: parameters
    QMetaType::Void, QMetaType::Double,    3,

 // slots: parameters
    QMetaType::Void, 0x80000000 | 5,    6,

       0        // eod
};

void AudioDecoder::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<AudioDecoder *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->clockChanged((*reinterpret_cast< double(*)>(_a[1]))); break;
        case 1: _t->onStateChanged((*reinterpret_cast< State(*)>(_a[1]))); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        {
            using _t = void (AudioDecoder::*)(double );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&AudioDecoder::clockChanged)) {
                *result = 0;
                return;
            }
        }
    }
}

QT_INIT_METAOBJECT const QMetaObject AudioDecoder::staticMetaObject = { {
    QMetaObject::SuperData::link<QThread::staticMetaObject>(),
    qt_meta_stringdata_AudioDecoder.data,
    qt_meta_data_AudioDecoder,
    qt_static_metacall,
    nullptr,
    nullptr
} };


const QMetaObject *AudioDecoder::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *AudioDecoder::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_AudioDecoder.stringdata0))
        return static_cast<void*>(this);
    return QThread::qt_metacast(_clname);
}

int AudioDecoder::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QThread::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 2)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 2;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 2)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 2;
    }
    return _id;
}

// SIGNAL 0
void AudioDecoder::clockChanged(double _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}
struct qt_meta_stringdata_VideoDecoder_t {
    QByteArrayData data[13];
    char stringdata0[120];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_VideoDecoder_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_VideoDecoder_t qt_meta_stringdata_VideoDecoder = {
    {
QT_MOC_LITERAL(0, 0, 12), // "VideoDecoder"
QT_MOC_LITERAL(1, 13, 12), // "frameDecoded"
QT_MOC_LITERAL(2, 26, 0), // ""
QT_MOC_LITERAL(3, 27, 8), // "uint8_t*"
QT_MOC_LITERAL(4, 36, 4), // "data"
QT_MOC_LITERAL(5, 41, 7), // "SwsSpec"
QT_MOC_LITERAL(6, 49, 4), // "spec"
QT_MOC_LITERAL(7, 54, 12), // "clockChanged"
QT_MOC_LITERAL(8, 67, 5), // "clock"
QT_MOC_LITERAL(9, 73, 14), // "onStateChanged"
QT_MOC_LITERAL(10, 88, 5), // "State"
QT_MOC_LITERAL(11, 94, 5), // "state"
QT_MOC_LITERAL(12, 100, 19) // "onAudioClockChanged"

    },
    "VideoDecoder\0frameDecoded\0\0uint8_t*\0"
    "data\0SwsSpec\0spec\0clockChanged\0clock\0"
    "onStateChanged\0State\0state\0"
    "onAudioClockChanged"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_VideoDecoder[] = {

 // content:
       8,       // revision
       0,       // classname
       0,    0, // classinfo
       4,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       2,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    2,   34,    2, 0x06 /* Public */,
       7,    1,   39,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
       9,    1,   42,    2, 0x0a /* Public */,
      12,    1,   45,    2, 0x0a /* Public */,

 // signals: parameters
    QMetaType::Void, 0x80000000 | 3, 0x80000000 | 5,    4,    6,
    QMetaType::Void, QMetaType::Double,    8,

 // slots: parameters
    QMetaType::Void, 0x80000000 | 10,   11,
    QMetaType::Void, QMetaType::Double,    8,

       0        // eod
};

void VideoDecoder::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<VideoDecoder *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->frameDecoded((*reinterpret_cast< uint8_t*(*)>(_a[1])),(*reinterpret_cast< SwsSpec(*)>(_a[2]))); break;
        case 1: _t->clockChanged((*reinterpret_cast< double(*)>(_a[1]))); break;
        case 2: _t->onStateChanged((*reinterpret_cast< State(*)>(_a[1]))); break;
        case 3: _t->onAudioClockChanged((*reinterpret_cast< double(*)>(_a[1]))); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        {
            using _t = void (VideoDecoder::*)(uint8_t * , SwsSpec );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&VideoDecoder::frameDecoded)) {
                *result = 0;
                return;
            }
        }
        {
            using _t = void (VideoDecoder::*)(double );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&VideoDecoder::clockChanged)) {
                *result = 1;
                return;
            }
        }
    }
}

QT_INIT_METAOBJECT const QMetaObject VideoDecoder::staticMetaObject = { {
    QMetaObject::SuperData::link<QThread::staticMetaObject>(),
    qt_meta_stringdata_VideoDecoder.data,
    qt_meta_data_VideoDecoder,
    qt_static_metacall,
    nullptr,
    nullptr
} };


const QMetaObject *VideoDecoder::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *VideoDecoder::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_VideoDecoder.stringdata0))
        return static_cast<void*>(this);
    return QThread::qt_metacast(_clname);
}

int VideoDecoder::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QThread::qt_metacall(_c, _id, _a);
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

// SIGNAL 0
void VideoDecoder::frameDecoded(uint8_t * _t1, SwsSpec _t2)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))), const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t2))) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void VideoDecoder::clockChanged(double _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE

#ifndef ENUMTYPE_H
#define ENUMTYPE_H

#include <QObject>

class EnumType: public QObject {
    Q_OBJECT

    Q_ENUMS(ModeStatus)
    Q_ENUMS(ShutdownButtonType)
    Q_ENUMS(KeyBoardButtonType)
    Q_ENUMS(TimeFormat)
    Q_ENUMS(NetState)
    Q_ENUMS(BatteryStatus)

public:
    enum ModeStatus {
        NoStatus,
        PowerMode
    };

    enum ShutdownButtonType {
        RebootButton = 0,
        ShutdownButton
    };

    enum KeyBoardButtonType {
        NumButton = 0,
        BackButton,
        ClearButton
    };

    enum AuthFlag {
        Password = 1 << 0,
        Fingerprint = 1 << 1,
        Face = 1 << 2,
        ActiveDirectory = 1 << 3
    };

    enum TimeFormat {
            ShortTime = 0,
            LongTime = 1
    };

    enum NetState {
        NM_DEVICE_STATE_UNKNOWN = 0,
        NM_DEVICE_STATE_UNMANAGED = 10,
        NM_DEVICE_STATE_UNAVAILABLE = 20,
        NM_DEVICE_STATE_DISCONNECTED = 30,
        NM_DEVICE_STATE_PREPARE = 40,
        NM_DEVICE_STATE_CONFIG = 50,
        NM_DEVICE_STATE_NEED_AUTH = 60,
        NM_DEVICE_STATE_IP_CONFIG = 70,
        NM_DEVICE_STATE_IP_CHECK = 80,
        NM_DEVICE_STATE_SECONDARIES = 90,
        NM_DEVICE_STATE_ACTIVATED = 100,
        NM_DEVICE_STATE_DEACTIVATING = 110,
        NM_DEVICE_STATE_FAILED = 120
    };

    enum BatteryStatus {
        StatusUnknown = 0,
        StatusCharging = 1,
        StatusDischarging = 2,
        StatusNotCharging = 3,
        StatusFull = 4
    };

    enum DeviceType {
        Wire = 1,
        Wireless = 2,
    };

public:
    EnumType(QObject *parent = nullptr);
    ~EnumType();
};

#endif // ENUMTYPE_H

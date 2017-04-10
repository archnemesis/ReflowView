#ifndef REFLOWPROTOCOL_H
#define REFLOWPROTOCOL_H

#include <QObject>
#include <QByteArray>

#include "reflowprofile.h"

#define PROTOCOL_MSG_START            0xAE

typedef enum {
    Standby = 1,
    Running = 2
} ReflowMode;

typedef enum {
    RampUp = 1,
    RampDown = 2,
    SetPoint = 3,
    Off = 4
} ReflowCommand;

typedef struct __attribute__ ((packed)) {
    uint8_t command_type;
    uint8_t command_arg1;
    uint8_t command_arg2;
    uint8_t command_arg3;
    uint8_t command_arg4;
} ReflowCommandPacket;

typedef struct __attribute__ ((packed)) {
    uint8_t mode;
    uint8_t status;
    uint32_t timer;
    float temperature;
} ReflowStatusPacket;

typedef struct __attribute__ ((packed)) {
    uint8_t slot;
    char name[16];
    uint16_t ramp_to_soak_rate;
    uint16_t soak_temp;
    uint16_t soak_time;
    uint16_t ramp_to_peak_rate;
    uint16_t peak_temp;
    uint16_t ramp_cooling_rate;
} ReflowStoreProfilePacket;

class ReflowProtocol : public QObject
{
    Q_OBJECT
public:
    typedef enum {
        StartCommand = 1,
        StopCommand,
        ClearMemoryCommand
    } CommandType;

    typedef enum {
        StandbyMode = 1,
        RunningMode
    } OvenMode;

    typedef enum {
        CommandMessage = 1,
        StoreProfileMessage,
        StatusMessage
    } MessageType;

    typedef enum {
        IdleStatus = 1,
        RampingToSoakStatus,
        SoakingStatus,
        RampingToPeakStatus,
        CoolingStatus,
        WaitingStatus
    } StatusType;

    explicit ReflowProtocol(QObject *parent = 0);
    void sendCommand(CommandType command);
    void storeReflowProfile(ReflowProfile &profile);

signals:
    void statusPacketReceived(ReflowStatusPacket packet);
    void emitBytes(QByteArray &bytes);

public slots:
    void inputBytes(QByteArray &bytes);

protected:
    QByteArray m_readBuffer;
    int m_packetBytes;
    int m_packetBytesExpected;
    int m_packetId;
    bool m_startOfPacketReceived;
    bool m_inPacket;
};

#endif // REFLOWPROTOCOL_H

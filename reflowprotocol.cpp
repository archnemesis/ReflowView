#include "reflowprotocol.h"

ReflowProtocol::ReflowProtocol(QObject *parent) :
    QObject(parent),
    m_packetBytes(0),
    m_packetBytesExpected(0),
    m_inPacket(false)
{

}

void ReflowProtocol::sendCommand(CommandType command)
{
    ReflowCommandPacket pkt;
    pkt.command_type = command;
    pkt.command_arg1 = 0;
    pkt.command_arg2 = 0;
    pkt.command_arg3 = 0;
    pkt.command_arg4 = 0;

    QByteArray data;
    data.append(PROTOCOL_MSG_START);
    data.append(CommandMessage);
    data.append((const char *)&pkt, sizeof(ReflowCommandPacket));
    emit emitBytes(data);
}

void ReflowProtocol::storeReflowProfile(ReflowProfile &profile)
{
    ReflowStoreProfilePacket pkt;
    pkt.slot = 0;
    pkt.ramp_to_soak_rate = profile.rampToSoakRate();
    pkt.soak_temp = profile.soakTemperature();
    pkt.soak_time = profile.soakTime();
    pkt.ramp_to_peak_rate = profile.rampToPeakRate();
    pkt.peak_temp = profile.peakTemperature();
    pkt.ramp_cooling_rate = profile.rampCoolingRate();
    strncpy((char *)pkt.name, profile.name().toLocal8Bit().constData(), 16);

    QByteArray data;
    data.append(PROTOCOL_MSG_START);
    data.append(StoreProfileMessage);
    data.append((const char *)&pkt, sizeof(ReflowStoreProfilePacket));
    emit emitBytes(data);
}

void ReflowProtocol::inputBytes(QByteArray &bytes)
{
    for (int i = 0; i < bytes.length(); i++) {
        unsigned char byte = (unsigned char)bytes.at(i);

        if (!m_startOfPacketReceived) {
            if (byte == PROTOCOL_MSG_START) {
                m_startOfPacketReceived = true;
            }
        }
        else {
            if (!m_inPacket) {
                switch (byte) {
                    case StatusMessage:
                    {
                        m_packetBytes = 0;
                        m_packetBytesExpected = sizeof(ReflowStatusPacket);
                        m_packetId = StatusMessage;
                        m_inPacket = true;
                        break;
                    }
                    default:
                    {
                        m_readBuffer.clear();
                        m_packetBytes = 0;
                        m_packetBytesExpected = 0;
                        m_startOfPacketReceived = false;
                        m_inPacket = false;
                        break;
                    }
                }
            }
            else {
                m_readBuffer.append(bytes.at(i));
                m_packetBytes++;

                if (m_packetBytes == m_packetBytesExpected) {
                    switch (m_packetId) {
                        case StatusMessage:
                        {
                            ReflowStatusPacket pkt;
                            memcpy((void *)&pkt, m_readBuffer.constData(), sizeof(ReflowStatusPacket));
                            emit statusPacketReceived(pkt);
                            break;
                        }
                    }

                    m_readBuffer.clear();
                    m_packetBytes = 0;
                    m_packetBytesExpected = 0;
                    m_inPacket = false;
                    m_startOfPacketReceived = false;
                }
            }
        }
    }
}

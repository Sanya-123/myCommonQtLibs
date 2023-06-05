#ifndef TCP_COMMUNICATIONQT_H
#define TCP_COMMUNICATIONQT_H

#include <a_communication.h>
#include <QTcpSocket>
#include "communicationdebuger.h"

#define DEFAULT_PORT            10002

class TCP_CommunicationQt : public A_Communication
{
public:
    TCP_CommunicationQt();

    void setDebuger(CommunicationDebuger *newDebuger);

    QTcpSocket *getSocket();

    //overwrite metods for communication
    bool openPort(std::string name);
    void closedPort();

    int writeData(uint8_t *data, uint32_t size);
    int readData(uint8_t *data, uint32_t size, uint32_t timeout);

    void resetRxBuffer();

    void setNoBlocked(bool noBloked);

    void delay_ms(uint32_t timeDelay_ms);

private:
    enum EventsTcpCommunication{
        ConnectedEvent,
        DisconnectedEvent,
        ByteWritedEvent,
        ReadyReadEvent,
    };
    bool waytEvent(enum EventsTcpCommunication event, uint32_t timeout_ms);

private:
    QTcpSocket socket;
    uint32_t timeout;
    CommunicationDebuger *debuger;
    bool noBlockedWayt;
};

#endif // TCP_COMMUNICATIONQT_H

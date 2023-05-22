#ifndef UARTCOMMUNICATIONQT_H
#define UARTCOMMUNICATIONQT_H

#include <QObject>
#include "a_communication.h"
#include <QSerialPort>
#include <string>
#include "communicationdebuger.h"

class UartCommunicationQt : public A_Communication/*, QObject*/
{
//    Q_OBJECT
public:
    explicit UartCommunicationQt(/*QObject *parent = nullptr*/);

    void setDebuger(CommunicationDebuger *newDebuger);

    //overwrite metods for communication
    bool openPort(std::string name);
    void closedPort();

    int writeData(uint8_t *data, uint32_t size);
    int readData(uint8_t *data, uint32_t size, uint32_t timeout);

    void resetRxBuffer();

    QSerialPort* getPort();

signals:

private:
    QSerialPort port;
    uint32_t timeout;
    CommunicationDebuger *debuger;
};

#endif // UARTCOMMUNICATIONQT_H

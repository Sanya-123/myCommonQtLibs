#include "uartcommunicationqt.h"
#include <cstring>
#include <QTimer>
#include <QDebug>
#include <QString>

UartCommunicationQt::UartCommunicationQt(/*QObject *parent*/)
    : /*QObject{parent}, */timeout(1000), debuger(nullptr)
{
    port.setBaudRate(QSerialPort::Baud115200);
}

void UartCommunicationQt::setDebuger(CommunicationDebuger *newDebuger)
{
    debuger = newDebuger;
}


bool UartCommunicationQt::openPort(std::string name)
{
    port.setPortName(QString(name.c_str()));
    return port.open(QIODevice::ReadWrite);
}

void UartCommunicationQt::closedPort()
{
    port.close();
}

int UartCommunicationQt::writeData(uint8_t *data, uint32_t size)
{
    if(!port.isOpen())
        return COMMUNICATION_ERROR_DEV;

    if(debuger)
        debuger->printTx( data, size);

    port.write((const char *)data, size);
    if(port.waitForBytesWritten(timeout))//if transfer is complete
        return COMMUNICATION_ERROR_NONE;

    return COMMUNICATION_ERROR_TIMEOUT;
}

int UartCommunicationQt::readData(uint8_t *data, uint32_t size, uint32_t timeout)
{
    if(!port.isOpen())
        return COMMUNICATION_ERROR_DEV;

    QByteArray resData = port.read(size);
    uint32_t _timeout = timeout;
    while(resData.size() < size)
    {
        if(port.waitForReadyRead(_timeout))
        {
            _timeout = timeout/10;
            resData.append(port.read(size - resData.size()));
        }
        else
            break;
    }

    if(debuger)
        debuger->printRx((uint8_t*)resData.data(), resData.size());

    if(resData.size() > size)
        memcpy(data, resData.data(), size);
    else
        memcpy(data, resData.data(), resData.size());

    return resData.size() == 0 ? COMMUNICATION_ERROR_TIMEOUT : resData.size();
}

void UartCommunicationQt::resetRxBuffer()
{
//    port.reset();
    port.readAll();
    while(port.waitForReadyRead(1)) {port.readAll();}
}

QSerialPort *UartCommunicationQt::getPort()
{
    return &port;
}

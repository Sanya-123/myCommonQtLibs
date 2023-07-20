#include "uartcommunicationqt.h"
#include <cstring>
#include <QTimer>
#include <QDebug>
#include <QString>
#include <QEventLoop>

UartCommunicationQt::UartCommunicationQt(/*QObject *parent*/)
    : /*QObject{parent}, */timeout(1000), debuger(nullptr), noBlockedWayt(false)
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
    releaseSemaphore();
    port.close();
}

int UartCommunicationQt::writeData(uint8_t *data, uint32_t size)
{
    if(!port.isOpen())
        return COMMUNICATION_ERROR_DEV;

    if(debuger)
        debuger->printTx( data, size);

    port.write((const char *)data, size);

    bool okWayt;
    if(noBlockedWayt)
        okWayt = waytEvent(ByteWritedEvent, timeout);
    else
        okWayt = port.waitForBytesWritten(timeout);

    if(okWayt)//if transfer is complete
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
        bool okWayt;
        if(noBlockedWayt)
            okWayt = waytEvent(ReadyReadEvent, _timeout);
        else
            okWayt = port.waitForReadyRead(_timeout);

        if(okWayt)
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

void UartCommunicationQt::setNoBlocked(bool noBloked)
{
    noBlockedWayt = noBloked;
}

void UartCommunicationQt::delay_ms(uint32_t timeDelay_ms)
{
    QTimer timerr;
    QEventLoop eventloop;
//    QObject::connect(&timerr, &QTimer::timeout, &eventloop, &QEventLoop::quit);
//    timerr.start(timeout_ms);
    timerr.singleShot(timeDelay_ms, &eventloop, &QEventLoop::quit);
    eventloop.exec();
}

bool UartCommunicationQt::getSemaphore(uint32_t timeOut)
{
    return sem.getSemaphore(timeOut);
}

void UartCommunicationQt::releaseSemaphore()
{
    sem.realeseSemaphore();
}

bool UartCommunicationQt::waytEvent(EventsUartCommunication event, uint32_t timeout_ms)
{
    QTimer timerr;
    QEventLoop eventloop;
//    QObject::connect(&timerr, &QTimer::timeout, &eventloop, &QEventLoop::quit);
    timerr.singleShot(timeout_ms, &eventloop, &QEventLoop::quit);

    switch(event)
    {
        case ByteWritedEvent: QObject::connect(&port, SIGNAL(bytesWritten(qint64)), &eventloop, SLOT(quit())); break;
        case ReadyReadEvent: QObject::connect(&port, SIGNAL(readyRead()), &eventloop, SLOT(quit())); break;
        default:
            break;
    }

    timerr.start(timeout_ms);
    eventloop.exec();
    bool res = timerr.isSingleShot();
//    QObject::disconnect(&timerr, &QTimer::timeout, &eventloop, &QEventLoop::quit);
    switch(event)
    {
        case ByteWritedEvent:
            QObject::disconnect(&port, SIGNAL(bytesWritten(qint64)), &eventloop, SLOT(quit()));
            res = port.bytesToWrite() == 0;
            break;
        case ReadyReadEvent:
            QObject::disconnect(&port, SIGNAL(readyRead()), &eventloop, SLOT(quit()));
            res = port.bytesAvailable() != 0;
            break;
        default:
            break;
    }

    return res;
}

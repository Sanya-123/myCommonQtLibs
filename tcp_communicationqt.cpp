#include "tcp_communicationqt.h"
#include <QEventLoop>
#include <QTimer>

TCP_CommunicationQt::TCP_CommunicationQt() : debuger(nullptr), timeout(1000), noBlockedWayt(false)
{

}

void TCP_CommunicationQt::setDebuger(CommunicationDebuger *newDebuger)
{
    debuger = newDebuger;
}

QTcpSocket *TCP_CommunicationQt::getSocket()
{
    return &socket;
}

bool TCP_CommunicationQt::openPort(std::string name)
{
//    QString _name(name.c_str());
    QList<QString> nameAndPort = QString(name.c_str()).split(":");

    if(nameAndPort.size() > 2)
        return false;

    uint16_t port = DEFAULT_PORT;
    if(nameAndPort.size() == 2)
    {
        port = nameAndPort[1].toUInt();
        if(port == 0)
            return false;
    }

    socket.connectToHost(nameAndPort[0], port);

    if(socket.waitForConnected(timeout))
        return true;

    return false;

}

void TCP_CommunicationQt::closedPort()
{
    socket.disconnectFromHost();
    socket.close();
}

int TCP_CommunicationQt::writeData(uint8_t *data, uint32_t size)
{
    if(!socket.isOpen())
        return COMMUNICATION_ERROR_DEV;

    if(debuger)
        debuger->printTx( data, size);

    socket.write((const char *)data, size);

    bool okWayt;
    if(noBlockedWayt)
        okWayt = waytEvent(ByteWritedEvent, timeout);
    else
        okWayt = socket.waitForBytesWritten(timeout);

    if(okWayt)//if transfer is complete
        return COMMUNICATION_ERROR_NONE;

    return COMMUNICATION_ERROR_TIMEOUT;
}

int TCP_CommunicationQt::readData(uint8_t *data, uint32_t size, uint32_t timeout)
{
    if(!socket.isOpen())
        return COMMUNICATION_ERROR_DEV;

    QByteArray resData = socket.read(size);
    uint32_t _timeout = timeout;
    while(resData.size() < size)
    {
        bool okWayt;
        if(noBlockedWayt)
            okWayt = waytEvent(ReadyReadEvent, _timeout);
        else
            okWayt = socket.waitForReadyRead(_timeout);

        if(okWayt)
        {
            _timeout = timeout/10;
            resData.append(socket.read(size - resData.size()));
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

void TCP_CommunicationQt::resetRxBuffer()
{
    socket.readAll();
    while(socket.waitForReadyRead(1)) {socket.readAll();}
}

void TCP_CommunicationQt::setNoBlocked(bool noBloked)
{
    noBlockedWayt = noBloked;
}

bool TCP_CommunicationQt::waytEvent(EventsTcpCommunication event, uint32_t timeout_ms)
{
    QTimer timerr;
    QEventLoop eventloop;
//    QObject::connect(&timerr, &QTimer::timeout, &eventloop, &QEventLoop::quit);
    timerr.singleShot(timeout_ms, &eventloop, &QEventLoop::quit);

    switch(event)
    {
        case ConnectedEvent: QObject::connect(&socket, SIGNAL(connected()), &eventloop, SLOT(quit())); break;
        case DisconnectedEvent: QObject::connect(&socket, SIGNAL(disconnected()), &eventloop, SLOT(quit())); break;
        case ByteWritedEvent: QObject::connect(&socket, SIGNAL(bytesWritten(qint64)), &eventloop, SLOT(quit())); break;
        case ReadyReadEvent: QObject::connect(&socket, SIGNAL(readyRead()), &eventloop, SLOT(quit())); break;
        default:
            break;
    }

    timerr.start(timeout_ms);
    eventloop.exec();
    bool res = timerr.isSingleShot();
//    QObject::disconnect(&timerr, &QTimer::timeout, &eventloop, &QEventLoop::quit);
    switch(event)
    {
        case ConnectedEvent:
            QObject::disconnect(&socket, SIGNAL(connected()), &eventloop, SLOT(quit()));
            res = socket.isOpen();
            break;
        case DisconnectedEvent:
            QObject::disconnect(&socket, SIGNAL(disconnected()), &eventloop, SLOT(quit()));
            res = true;
            break;
        case ByteWritedEvent:
            QObject::disconnect(&socket, SIGNAL(bytesWritten(qint64)), &eventloop, SLOT(quit()));
            res = socket.bytesToWrite() == 0;
            break;
        case ReadyReadEvent:
            QObject::disconnect(&socket, SIGNAL(readyRead()), &eventloop, SLOT(quit()));
            res = socket.bytesAvailable() != 0;
            break;
        default:
            break;
    }

    return res;
}

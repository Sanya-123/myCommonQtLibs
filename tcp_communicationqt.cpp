#include "tcp_communicationqt.h"

TCP_CommunicationQt::TCP_CommunicationQt() : debuger(nullptr), timeout(1000)
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
    if(socket.waitForBytesWritten(timeout))//if transfer is complete
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
        if(socket.waitForReadyRead(_timeout))
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

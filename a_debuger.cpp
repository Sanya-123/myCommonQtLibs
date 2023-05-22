#include "a_debuger.h"


A_Debuger::A_Debuger(QTextEdit *loger, QObject *parent) :
    QObject(parent), textEdit(loger), autoScroll(true)
{

}

A_Debuger::~A_Debuger()
{

}

QString A_Debuger::getErrorString(int error)
{
    QString res = tr("No Error");
    switch(error)
    {
        case 0: break;
        case -1: res = tr("Device Error"); break;
        case -2: res = tr("Timeout"); break;
        case -3: res = tr("Wrond size respone"); break;
        case -4: res = tr("CRC Error"); break;

        default: res = tr("Unnounn Error"); break;
    }
    return res;
}

void A_Debuger::setLogger(QTextEdit *newTextEdit)
{
    textEdit = newTextEdit;
}

QString A_Debuger::getErrorMsg(int error)
{
    return MessageDebuger::beginError + QString::asprintf("err:%d - ", error) + getErrorString(error) + MessageDebuger::end;
}

void A_Debuger::setAutoScroll(bool newAutoScroll)
{
    autoScroll = newAutoScroll;
}

bool A_Debuger::getAutoScroll() const
{
    return autoScroll;
}

void A_Debuger::printArray(QString header, uint8_t *data, uint32_t size)
{
    if(textEdit == nullptr)
        return;

    textEdit->append(header);
    QString msg = MessageDebuger::beginData;
    for(uint32_t i = 0; i < size; i++)
    {
        msg += QString::asprintf("0x%02X ", data[i]);
    }

    msg += MessageDebuger::end;
    textEdit->append(msg);
    textEdit->append("");

    if(autoScroll)
        textEdit->moveCursor(QTextCursor::End);
}

void A_Debuger::printArray(QString header, QByteArray data)
{
    printArray(header, (uint8_t*)data.data(), data.size());
}

void A_Debuger::printError(int error)
{
    if(textEdit == nullptr)
        return;

    QString msg = MessageDebuger::beginError + QString::asprintf("err:%d - ", error) + getErrorString(error) + MessageDebuger::end;
    textEdit->append(msg);
    textEdit->append("");

    if(autoScroll)
        textEdit->moveCursor(QTextCursor::End);
}

void A_Debuger::printError(QString error)
{
    if(textEdit == nullptr)
        return;
    textEdit->append(MessageDebuger::beginError + error + MessageDebuger::end);
    textEdit->append("");

    if(autoScroll)
        textEdit->moveCursor(QTextCursor::End);
}

void A_Debuger::print(QString msg)
{
    if(textEdit == nullptr)
        return;

    textEdit->append(msg);

    if(autoScroll)
        textEdit->moveCursor(QTextCursor::End);
}


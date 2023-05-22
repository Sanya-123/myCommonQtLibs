#include "communicationdebuger.h"
#include "ui_communicationdebuger.h"

CommunicationDebuger::CommunicationDebuger(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::CommunicationDebuger), communication(nullptr)
{
    ui->setupUi(this);

    debuger.setLogger(ui->textEdit_log);
}

CommunicationDebuger::~CommunicationDebuger()
{
    delete ui;
}

A_Debuger *CommunicationDebuger::getDebuger()
{
    return &debuger;
}

void CommunicationDebuger::setCommunication(A_Communication *newCommunication)
{
    communication = newCommunication;
}

void CommunicationDebuger::sendData(QByteArray data)
{
    if(communication == nullptr)
        return;

    communication->writeData((uint8_t*)data.data(), data.size());
    data.resize(16384);
    int res = communication->readData((uint8_t*)data.data(), data.size(), 1000);
    if(res < 0)
    {
        ui->textEdit_log->append(A_Debuger::getErrorMsg(res));
        ui->textEdit_log->append("");
        if(debuger.getAutoScroll())
            ui->textEdit_log->moveCursor(QTextCursor::End);
    }
}

void CommunicationDebuger::printTx(uint8_t *data, uint32_t size)
{
    if(ui->checkBox_enTxDebug->isChecked())
        debuger.printArray(tr("TX data:"), data, size);
}

void CommunicationDebuger::printRx(uint8_t *data, uint32_t size)
{
    if(ui->checkBox_enRxDebug->isChecked())
        debuger.printArray(tr("RX data:"), data, size);
}

void CommunicationDebuger::on_pushButton_sendCustomCommand_clicked()
{
    QList<QString> data = ui->lineEdit_data->text().simplified().split(' ');

    QByteArray txBytes;

    if(!ui->lineEdit_data->text().simplified().isEmpty())
    for(int i = 0; i < data.size(); i++)
    {
        bool okInput;
        txBytes.append(data[i].toUInt(&okInput, 16));
        if(!okInput)
        {
            ui->textEdit_log->append(MessageDebuger::beginError + "Error input data" + MessageDebuger::end);
            ui->textEdit_log->append("");
            if(debuger.getAutoScroll())
                ui->textEdit_log->moveCursor(QTextCursor::End);
            return;
        }
    }

    ui->textEdit_log->append(tr("Send ") + QString::number(txBytes.size()) + " bytes");

    if(debuger.getAutoScroll())
        ui->textEdit_log->moveCursor(QTextCursor::End);

    sendData(txBytes);
}

void CommunicationDebuger::on_checkBox_autoScroll_toggled(bool checked)
{
    debuger.setAutoScroll(checked);
}


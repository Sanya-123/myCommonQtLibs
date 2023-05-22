#ifndef COMMUNICATIONDEBUGER_H
#define COMMUNICATIONDEBUGER_H

#include <QWidget>
#include "a_debuger.h"
#include "a_communication.h"
#include <QTextEdit>

namespace Ui {
class CommunicationDebuger;
}

class CommunicationDebuger : public QWidget
{
    Q_OBJECT

public:
    explicit CommunicationDebuger(QWidget *parent = nullptr);
    ~CommunicationDebuger();
    A_Debuger *getDebuger();

    void setCommunication(A_Communication *newCommunication);

    virtual void sendData(QByteArray data);

public slots:
    void printTx(uint8_t *data, uint32_t size);
    void printRx(uint8_t *data, uint32_t size);

private slots:
    void on_pushButton_sendCustomCommand_clicked();

    void on_checkBox_autoScroll_toggled(bool checked);

private:
    Ui::CommunicationDebuger *ui;
    A_Communication *communication;
    A_Debuger debuger;
};

#endif // COMMUNICATIONDEBUGER_H

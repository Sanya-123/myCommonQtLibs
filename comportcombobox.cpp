#include "comportcombobox.h"
#include <QDebug>
#include <QSerialPortInfo>

ComPortComboBox::ComPortComboBox(QWidget *parent) : QComboBox(parent)
{
//    foreach (const QSerialPortInfo &info, QSerialPortInfo::availablePorts())//цикл поиска всех портов
    QList<QSerialPortInfo> infoList = QSerialPortInfo::availablePorts();
    for(int i = 0; i < infoList.size(); i++)
    {
        addItem(infoList[i].portName());//добаления
    }
}

ComPortComboBox::~ComPortComboBox()
{

}

void ComPortComboBox::mousePressEvent(QMouseEvent *e)
{
    QString oldCurrentIndexMaster;
    //если был выбран какойто индекс чтобы он остался
    if(currentIndex() > 0)
        oldCurrentIndexMaster = currentText();

    clear();
//    foreach (const QSerialPortInfo &info, QSerialPortInfo::availablePorts())//цикл поиска всех портов
    QList<QSerialPortInfo> infoList = QSerialPortInfo::availablePorts();
    for(int i = 0; i < infoList.size(); i++)
    {
        addItem(infoList[i].portName());//добаления
    }

    //выставляю индекс обратно
    setCurrentText(oldCurrentIndexMaster);
    return QComboBox::mousePressEvent(e);
}

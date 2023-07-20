#include "binsemaphoreqt.h"
#include <QEventLoop>
#include <QTimer>

BinSemaphoreQt::BinSemaphoreQt(QObject *parent)
    : QObject{parent}, availeble(true)
{

}

bool BinSemaphoreQt::getSemaphore(uint32_t timeOut)
{
    //if semaphore is note availeble

    if(!availeble)
    {
        QTimer timerr;
        QEventLoop eventloop;
        timerr.singleShot(timeOut, &eventloop, &QEventLoop::quit);
        QObject::connect(this, SIGNAL(realesedSemaphore()), &eventloop, SLOT(quit()));
    }


    if(availeble)
    {
        availeble = false;
        return true;
    }
    return false;
}

void BinSemaphoreQt::realeseSemaphore()
{
    availeble = true;
    emit realesedSemaphore();
}

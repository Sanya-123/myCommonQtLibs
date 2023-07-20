#ifndef BINSEMAPHOREQT_H
#define BINSEMAPHOREQT_H

#include <QObject>

class BinSemaphoreQt : public QObject
{
    Q_OBJECT
public:
    explicit BinSemaphoreQt(QObject *parent = nullptr);
    bool getSemaphore(uint32_t timeOut = 0xFFFFFFFF);

public slots:
    void realeseSemaphore();

signals:
    void realesedSemaphore();

private:
    bool availeble;

};

#endif // BINSEMAPHOREQT_H

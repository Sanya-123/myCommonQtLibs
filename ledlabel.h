#ifndef LEDLABEL_H
#define LEDLABEL_H

#include <QLabel>

class LedLabel : public QLabel
{
    Q_OBJECT
public:
    explicit LedLabel(QWidget *parent = 0);

    enum State{
            StateOk,
            StateOkBlue,
            StateWarning,
            StateError
        };


    signals:

    public slots:
        void setState(State state);
        void setState(bool state);
};

#endif // LEDLABEL_H

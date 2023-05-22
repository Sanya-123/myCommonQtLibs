#ifndef COMBOBOX_H
#define COMBOBOX_H

#include <QObject>
#include <QComboBox>
#include <QMouseEvent>

class ComPortComboBox : public QComboBox
{
public:
    ComPortComboBox(QWidget *parent = 0);
    ~ComPortComboBox();

signals:
    void openMenu();

private:
    void mousePressEvent(QMouseEvent *e);

};

#endif // COMBOBOX_H

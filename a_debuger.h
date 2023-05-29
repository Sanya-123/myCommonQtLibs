#ifndef A_DEBUGER_H
#define A_DEBUGER_H

#include <QTextEdit>

namespace MessageDebuger {
static const QString beginError("<html><head/><body><span style=\" color:#aa0000;\">");
static const QString beginData("<html><head/><body><span style=\" color:#0055ff;\">");
static const QString end("</span></body></html>");
}

class A_Debuger : public QObject
{
    Q_OBJECT
public:
    explicit A_Debuger(QTextEdit *loger = nullptr, QObject *parent = nullptr);
    ~A_Debuger();
    static QString getErrorString(int error);

    void setLogger(QTextEdit *newTextEdit);

    static QString getErrorMsg(int error);

    void setAutoScroll(bool newAutoScroll);

    bool getAutoScroll() const;

public slots:
    void printArray(QString header, uint8_t *data, uint32_t size);
    void printArray(QString header, QByteArray data);
    void printError(int error);
    void printError(QString error);
    void print(QString msg);

signals:
    void printedError(QString msg);

private:
    QTextEdit *textEdit;
    bool autoScroll;

};

#endif // A_DEBUGER_H

#ifndef SERIALCOMM_H
#define SERIALCOMM_H

#include <QObject>
#include <QDebug>
class serialComm : public QObject
{
    Q_OBJECT
public:
    explicit serialComm(QObject *parent = nullptr);

    Q_INVOKABLE QString readSerial(bool Mode);

signals:

private slots:
    void serialDataReceived();
};

#endif // SERIALCOMM_H

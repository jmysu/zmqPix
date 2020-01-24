#ifndef ZPUB_H
#define ZPUB_H
#include <QObject>
#include <QHostAddress>
#include <QDebug>
#include <zmq.h>

class Publisher : public QObject
{
    Q_OBJECT
public:
    Publisher(int iport, QObject * parent = nullptr);
    ~Publisher();

    void initZMQ(int iport);
    void sendMsg(const QByteArray &ba);

    QHostAddress addrPublisher;

signals:

private:
    void * ptrContextPublisher;
    void * ptrSocketPublisher;
};

#endif // ZPUB_H

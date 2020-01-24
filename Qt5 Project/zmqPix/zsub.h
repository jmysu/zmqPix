#ifndef ZSUB_H
#define ZSUB_H
#include <QObject>
#include <QHostAddress>
#include <zmq.h>
#include <QJsonDocument>
#include <QJsonObject>

class Subscriber : public QObject
{
    Q_OBJECT
public:
    Subscriber(QHostAddress a, int iport, QObject * parent = nullptr);
    ~Subscriber();

    void initZMQ(QHostAddress a, int iport);
    void disconnect(QHostAddress addr, int port);

    int getSubDataNoBlock(QByteArray &ba);
    int getSubDataBlockedMultiPart(QByteArray &ba);

    QHostAddress addrSubscriber;

signals:
    void sigSubMsg(QByteArray ba);

private:
    void * ptrContextSubscriber;
    void * ptrSocketSubscriber;
 };

#endif // ZSUB_H

#ifndef MYTHREAD_H
#define MYTHREAD_H
#include "zpub.h"
#include "zsub.h"

#include <QApplication>
#include <QThread>
#include <QJsonDocument>
#include <QJsonObject>
#include <QTime>
#include <QTimer>
#include <QObject>
#include <QMutex>
#include <QDebug>

class ZThread : public QThread
{
    Q_OBJECT
public:
    ZThread();

    void changeRunState(bool run);
    bool getRunState();

    void changeHBinterval(int iInterval);
    int iHBinterval=5; //5sec
    int iHBcount=0;
    void sendHeartBeat();

    int iPortPublisher=5556;
    int iPortSubscriber=5556;
    //QString sAddrPublisher="192.168.0.14";
    //QString sAddrSubscriber="192.168.0.37";
    QString sAddrPublisher="localhost";
    QString sAddrSubscriber="127.0.0.1";

    Subscriber *pSubscriber;
    Publisher *pPublisher;

signals:
    void sigHBcount(int);

protected:
    virtual void run();

private:
    volatile bool isRun;
};

#endif // MYTHREAD_H

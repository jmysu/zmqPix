/*
 *  QThread main program for zmq pub/sub
 */
#include "zThread.h"



ZThread::ZThread()
{
    isRun = false;

    pPublisher = new Publisher(iPortPublisher);
    pSubscriber = new Subscriber(QHostAddress(sAddrSubscriber), iPortSubscriber);


    QTimer *timer = new QTimer(this);
    iHBinterval = 5;
    timer->start(iHBinterval*1000);
    connect(timer, &QTimer::timeout, [=]() {
        if (isRun) {
            sendHeartBeat(); //iHBcount increased here
            timer->setInterval(iHBinterval*1000); //change interval when updates available
            emit sigHBcount(iHBcount);
            }
        } );
}

void ZThread::changeRunState(bool run)
{
    isRun = run;
    qDebug() << __FUNCTION__ << isRun;
    if (isRun && (!isRunning())) start();
}

bool ZThread::getRunState()
{
    return isRun;
}


void ZThread::changeHBinterval(int iInterval)
{
    iHBinterval = iInterval;
}

void ZThread::sendHeartBeat()
{
    QJsonObject jsonObj {
            {"Name", qApp->applicationName()},
            {"OS",   QSysInfo::prettyProductName()},
            {"Time", QTime::currentTime().toString()},
            {"HB", iHBcount++}
            };
    QJsonDocument doc(jsonObj);
    //qDebug() << __FUNCTION__ << doc;

    QByteArray ba = doc.toJson(QJsonDocument::Compact);

    pPublisher->sendMsg(ba);
    //emit pPublisher->sigPublished(ba);
}

void ZThread::run()
{
QByteArray ba;

    while (1) {
        if (!isRun) return;

        ba.clear();
        //if (pSubscriber->getSubDataNoBlock(ba)){
        if (pSubscriber->getSubDataBlockedMultiPart(ba)){
            qDebug() << "[zthread] got" << ba.size();
            emit pSubscriber->sigSubMsg(ba);
            }
        }
}

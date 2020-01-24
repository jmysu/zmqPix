#include "zpub.h"

//////////////////////////////////////////////////////////////////////////
//Publisher::Publisher(QHostAddress addr, int port, QObject * parent /*= nullptr*/)
Publisher::Publisher(int port, QObject * parent /*= nullptr*/)
    :QObject(parent)
{
    initZMQ(port);
}

Publisher::~Publisher()
{
    zmq_close(ptrSocketPublisher);
    zmq_ctx_destroy(ptrContextPublisher);
}

void Publisher::initZMQ(int port)
{
    addrPublisher = QHostAddress::LocalHost;
    //iPortPublisher = port;

    ptrContextPublisher = zmq_ctx_new();
    ptrSocketPublisher = zmq_socket(ptrContextPublisher, ZMQ_PUB);

    QString sTCP = QString("tcp://*:%1").arg(port);
    int rc = zmq_bind(ptrSocketPublisher, qPrintable(sTCP)); //This also works! :)
    if (rc != 0){
        int err = zmq_errno();
        qWarning() << "zmqMan Publisher Bind Error" << err << zmq_strerror(err);
        }
    else {
        qDebug() << Q_FUNC_INFO << sTCP;
        }
    assert(rc == 0);
}


void Publisher::sendMsg(const QByteArray &ba)
{
    int ret = zmq_send(ptrSocketPublisher, ba.data(), ba.size(), 0);
    qDebug() << "\n";
    qDebug().noquote() << __FUNCTION__ << "[zpub] sent:" << ret;
    //qDebug().noquote() << Q_FUNC_INFO << ret << ba.toHex();
}

#include "zsub.h"

//////////////////////////////////////////////////////////////////////////
Subscriber::Subscriber(QHostAddress a, int iport, QObject * parent /*= nullptr*/)
    : QObject(parent)
{
    initZMQ(a, iport);
}

Subscriber::~Subscriber()
{
    zmq_close(ptrSocketSubscriber);
    zmq_ctx_destroy(ptrContextSubscriber);
    qDebug() << __FUNCTION__;
}

void Subscriber::disconnect(QHostAddress addr, int port)
{
    addrSubscriber = addr;
    //iPortSubscriber = port;

    ptrContextSubscriber = zmq_ctx_new();
    ptrSocketSubscriber = zmq_socket(ptrContextSubscriber, ZMQ_SUB);

    QString sTCP = QString("tcp://%1:%2").arg(addrSubscriber.toString()).arg(port);
    qDebug()<<__FUNCTION__<< sTCP;
    int rc = zmq_disconnect(ptrSocketSubscriber, qPrintable(sTCP)); //This Works! :)
    if (rc != 0){
        int err = zmq_errno();
        qWarning() << "zmqMan Subscriber disconnect Error" << err << zmq_strerror(err);
        //return;
        }
    else {
        qDebug() << Q_FUNC_INFO << sTCP;
        }
    assert(rc == 0);
}

void Subscriber::initZMQ(QHostAddress addr, int port)
{
    addrSubscriber = addr;
    //iPortSubscriber = port;

    ptrContextSubscriber = zmq_ctx_new();
    ptrSocketSubscriber = zmq_socket(ptrContextSubscriber, ZMQ_SUB);

    QString sTCP = QString("tcp://%1:%2").arg(addrSubscriber.toString()).arg(port);
    int rc = zmq_connect(ptrSocketSubscriber, qPrintable(sTCP)); //This Works! :)
    if (rc != 0){
        int err = zmq_errno();
        qWarning() << "zmqMan Subscriber Connect Error" << err << zmq_strerror(err);
        //return;
        }
    else {
        qDebug() << Q_FUNC_INFO << sTCP;
        }
    assert(rc == 0);

    /*zmq_setsockopt()函数会对socket参数指定的socket进行设置，
    设置的属性由option_name参数指定，属性值由参数option_value指定。
    option_len参数指定属性值的数据存储空间的大小
    option: 查看函数翻译文档 https://www.cnblogs.com/fengbohello/p/4398953.html
    */
    int ret = zmq_setsockopt(ptrSocketSubscriber, ZMQ_SUBSCRIBE, nullptr, 0);
    qDebug() << Q_FUNC_INFO << "SocketOpt" << ret;
}

/*
Receiving a multi-part message
*/
int Subscriber::getSubDataBlockedMultiPart(QByteArray &ba)
{
int nbytes = 0;
    while (1){
        zmq_msg_t message;
        zmq_msg_init (&message);
        nbytes = zmq_msg_recv (&message, ptrSocketSubscriber, 0);
        //  Process the message frame
        qDebug() << __FUNCTION__ << nbytes;
        ba.append((char*)zmq_msg_data(&message),nbytes);
        zmq_msg_close (&message);

        int more;
        size_t more_size = sizeof (more);
        zmq_getsockopt (ptrSocketSubscriber, ZMQ_RCVMORE, &more, &more_size);
        if (!more)
            break;      //  Last message frame
        }
    return nbytes;
}

int Subscriber::getSubDataNoBlock(QByteArray &ba)
{
char buffer[256];
int nbytes = 0;

    memset(buffer, 0, 256 * sizeof(char));
    nbytes = zmq_recv(ptrSocketSubscriber, &buffer, 256, ZMQ_NOBLOCK);
    if (nbytes == -1) { //noting got, just return
        //qDebug() << "receiver cmd null , continue";;
        return 0;
        }
    //Convert message to ByteArray
    //QByteArray ba;
    ba.append(buffer);
    qDebug() << Q_FUNC_INFO << ba.toHex();

    return nbytes;
}

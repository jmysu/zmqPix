/*
 *
 *  libZmq pub/sub QPixmap
 *
 */
#include "mainwindow.h"
#include "ui_mainwindow.h"

QString sMyIP(){
       QString localhostIP;
       QList<QHostAddress> alllist = QNetworkInterface::allAddresses();
       qDebug() << "All list:" << alllist;
       foreach (const QHostAddress& address, alllist) {
           if ((address.protocol() == QAbstractSocket::IPv4Protocol) && (!address.isLoopback())) {
                localhostIP = address.toString();
                }
            }
       return localhostIP;
}

//
//
//
QJsonValue jsonValFromPixmap(const QPixmap &p) {
  QBuffer buffer;
  buffer.open(QIODevice::WriteOnly);
  p.save(&buffer, "PNG");
  auto const encoded = buffer.data().toBase64();
  qDebug() << __FUNCTION__ <<encoded;
  //return {QLatin1String(encoded)};
  return qUtf8Printable(encoded);
}

//
//
//
QPixmap pixmapFromJsonValue(const QJsonValue &val) {
  auto const encoded = val.toString().toLatin1();
  QPixmap p;
  p.loadFromData(QByteArray::fromBase64(encoded), "PNG");
  return p;
}

///////////////////////////////////////////////////////////////////////////////
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    this->setWindowTitle(qApp->applicationName()+" @"+sMyIP());

    //Connect sendTimer for stress test
    connect(&sendTimerN, SIGNAL(timeout()), this, SLOT(on_pushButtonSend_clicked()));

    //Load GIF
    myGif = new QMovie(":/image/Trump.gif");
    ui->labelPixSend->resize(ui->frame->size().width()/2, ui->frame->size().height());
    myGif->setScaledSize(ui->labelPixSend->size());
    ui->labelPixSend->setMovie(myGif);
    myGif->start();

    /*Init Threads*/
    zthread = new ZThread;
    connect(zthread,SIGNAL(finished()),this,SLOT(slotThreadFinished()));
    //connect(zthread,SIGNAL(sigHBcount(int)) , this, SLOT(slotHBcount(int)));

    connect(this, &MainWindow::sigPubMsg, zthread->pPublisher, &Publisher::sendMsg);
    connect(zthread->pSubscriber, &Subscriber::sigSubMsg, this, &MainWindow::slotSubMsg);

    zthread->changeRunState(true);
    qDebug() << "ZThread isRun" << zthread->getRunState();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::slotThreadFinished()
{
    qDebug()<<__FUNCTION__<< tr("finished trigged!");
}

void MainWindow::slotSubMsg(QByteArray ba)
{
qDebug() << __FUNCTION__ << ba.size();

    if (ba.startsWith("{")) { //JSON document
        QJsonParseError jsonError;
        QJsonDocument jdoc = QJsonDocument::fromJson(ba, &jsonError);
        if( jsonError.error != QJsonParseError::NoError )
             qDebug() << QString("Json error: %1").arg(jsonError.errorString());
        else if( jdoc .isNull() )
             qDebug() << "Null JSON...";

        //qDebug() << "JSON:" << jdoc.toJson(QJsonDocument::Compact);
        //Parse JSON
        QJsonObject jobj = jdoc.object();
        if (jobj.contains("HB")) {
            qDebug()<< "HearBeat:" << jobj["HB"].toInt();
            QString sInfo = QString("HeartBeat (%1)").arg(jobj["HB"].toInt());
            ui->labelStatusRecv->setText(sInfo);
            }
        }
    else if (ba.contains("PNG")) {
        QPixmap img;
        img.loadFromData(ba);
        qDebug() << "QPixmap:" << ba.size();
        newImageReceived(img);
        }
}

void MainWindow::on_pushButtonSend_clicked()
{
    QPixmap pm;
    if (m_imagePath.isEmpty() && myGif->isValid())
        pm = myGif->currentPixmap();
    else
        pm = *ui->labelPixSend->pixmap();
    if (!pm) return; //no pixmap

    QByteArray bArray;
    QBuffer buffer(&bArray);
    buffer.open(QIODevice::WriteOnly);
    pm.save(&buffer, "PNG");

    emit sigPubMsg(bArray);

    timeElapsed.start();
    QString sInfo = QString("%1 (%2x%3)").arg(m_imagePath).arg(pm.width()).arg(pm.height());
    ui->labelStatusSend->setText(sInfo);
}

void MainWindow::on_pushButtonLoad_clicked()
{
    m_imagePath = QFileDialog::getOpenFileName(this, tr("Open File"),"", tr("JPEG/PNG (*.jpg *.jpeg *.png)" ) );
    if (m_imagePath.isEmpty()) return;

    QImage image;
    bool resultOk = image.load(m_imagePath);
    if (resultOk)
        ui->labelPixSend->setPixmap(QPixmap::fromImage(image));
    QString sInfo = QString("%1 (%2x%3)").arg(m_imagePath).arg(image.width()).arg(image.height());
    ui->labelStatusSend->setText(sInfo);

    iRecvCnt=0;
}

void MainWindow::newImageReceived(QPixmap pm)
{
    if (!pm) return;
    float iSize = pm.width()*pm.height()*pm.depth()/(8*1024*1024.0);
    float iTimeElapsed = timeElapsed.elapsed()/1000.0;
    ui->labelStatusRecv->clear();
    ui->labelPixRecv->setPixmap(pm);
    QString sInfo = QString("Got Pixmap(%1x%2)@%3MB/s x%4").arg(pm.width()).arg(pm.height()).arg(iSize/iTimeElapsed, 4,'f',1).arg(++iRecvCnt);
    ui->labelStatusRecv->setText(sInfo);
}

void MainWindow::on_pushButtonSendTimer_clicked()
{
    (sendTimerN.isActive() ? sendTimerN.stop() : sendTimerN.start(ui->spinBoxTimer->value()) );

    qApp->processEvents();//Important
    qDebug() << "sendTimerN active?" <<  sendTimerN.isActive();
    if (sendTimerN.isActive()==false){
        ui->pushButtonSendTimer->setText("Send N");
        }
    else {
        ui->pushButtonSendTimer->setText("Stop");
        }
}

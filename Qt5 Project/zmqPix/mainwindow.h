/*
 *
 *  libZmq pub/sub QPixmap
 *
 */
#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QNetworkInterface>
#include <QHostAddress>
#include <QMessageBox>
#include <QFileDialog>
#include <QBuffer>
#include <QTimer>
#include <QTime>
#include <QMovie>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>

#include "zThread.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

    QTimer sendTimerN;
    QTime  timeElapsed;
    QMovie *myGif;
    QString m_imagePath;

signals:
    void sigPubMsg(const QByteArray &ba);

public slots:
    void slotSubMsg(QByteArray ba);

private slots:
    void on_pushButtonSend_clicked();
    void on_pushButtonLoad_clicked();
    void newImageReceived(QPixmap pm);

    void on_pushButtonSendTimer_clicked();
    void slotThreadFinished();

private:
    Ui::MainWindow *ui;

    int iRecvCnt=0;

    ZThread *zthread;
};

#endif // MAINWINDOW_H

#ifndef SERVER_H
#define SERVER_H

#include "heads.h"
#include <QtNetwork>
#include <QMessageBox>
#include <QWidget>
#include <QMap>
#include <QList>
#include<QTimer>
#include<dsheet.h>
#include <QSqlDatabase>
#include <QSqlError>
#include <QSqlQuery>
#include <QDebug>
#define zhire "W"
#define zhileng "C"
#define mid "M"
#define high "H"
#define low "L"
#define fee_mid 0.2
#define fee_low 0.1
#define fee_high 0.3
#define ope "O"
#define clo "S"
#define updat "U"
#define schedule_stall 2
#define rate_low 0.1
#define rate_mid 0.2
#define rate_high 0.3
#define que_max 2

class Servered:  QObject{
    Q_OBJECT
public:
    Servered(){}
    void start();
    void end();
    int time;
    int op_time=0;
    QString id;
    bool X=false;
    QTimer *t;
public slots:
    int work();
};

class Scheduled:  QObject{
    Q_OBJECT
public:
    Scheduled(){}
    int time;
    QString id;

    QTimer *t;
    void start();
    void end();
public slots:
     void timer();
};



namespace Ui {
class Server;
}

class Server : public QWidget
{
    Q_OBJECT

public:
    explicit Server(QWidget *parent = 0);
    void send_message(int number,QString id);
    void read(QString id, room tmp);

    void insert_bill(QString id,QString op);
    void update_bill(QString id);
    void init_db();
    void generate_bill(QString id);
    void addone(QString id);


    ~Server();
public slots:
    void slotNewConnection();
    void slotReadyRead();
    void slotAcceptError();
    void slotDestroyed();

private slots:
    void on_pushButton_clicked();
    void slot_Disconnected();
    void on_button_generatebill_1_clicked();


private:
    Ui::Server *ui;
    QTcpServer * m_server=new QTcpServer();
    QTcpSocket * m_client=new QTcpSocket();
    QList<QTcpSocket *> list_client;
    QByteArray message;
    int standard;
    QSqlDatabase database;
signals:
    void sendData(room);
};




#endif // SERVER_H

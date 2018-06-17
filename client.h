#ifndef CLIENT_H
#define CLIENT_H
#include <QtNetwork>
#include <QMessageBox>
#include <iostream>
#include <QDebug>
#include <QWidget>
#include <QtCore>
#include <server.h>
#define stall 1000
using namespace std;

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

namespace Ui {
class Client;
}

class Client : public QWidget
{
    Q_OBJECT

public:
    explicit Client(QWidget *parent = 0);
    void initRoom(string id);
    void write_obj(QString my_operator);
    void huiwen();
    ~Client();

public slots:
    void slotReadyRead();
    void slot_send();
    void slotSendOnOffMsg();

    void slotDisconnected();

   // void slotSendotherMsg();
private slots:
    void on_button_On_Off_clicked();

    void on_button_mode_cold_clicked();

    void on_button_mode_hot_clicked();

    void on_button_speed_low_clicked();

    void on_button_speed_middle_clicked();

    void on_button_speed_high_clicked();

//    void on_button_up_1_clicked();

//    void on_button_down_1_clicked();

    void on_button_checkout_clicked();


    void on_Button_changeT_clicked();

private:
    Ui::Client *ui;
    QTcpSocket* m_client;
    QTimer *timer;
    QTimer *timer2;
    QJsonObject request_On_Obj;
    QString state;
    QString gear;
    float temperature;
    float fee;
    int cflag;
};

#endif // CLIENT_H

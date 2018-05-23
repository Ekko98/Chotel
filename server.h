#ifndef SERVER_H
#define SERVER_H

#include <QtNetwork>
#include <QMessageBox>
#include <QWidget>


namespace Ui {
class Server;
}

class Server : public QWidget
{
    Q_OBJECT

public:
    explicit Server(QWidget *parent = 0);
    send_message(QByteArray message,int number);
    ~Server();
public slots:
    void slotNewConnection();
    void slotReadyRead();
    void slotAcceptError();
    void slotDestroyed();
    void slot_Disconnected();
private:
    Ui::Server *ui;
    QTcpServer * m_server;
    QTcpSocket * m_client;
    QList<QTcpSocket *> list_client;
    QByteArray message;
};

#endif // SERVER_H

#ifndef SERVER_H
#define SERVER_H

#include<QtNetwork>
#include <QWidget>
#include <QMessageBox>

namespace Ui {
class Server;
}

class Server : public QWidget
{
    Q_OBJECT

public:
    explicit Server(QWidget *parent = 0);
    ~Server();
public slots:
    void slotNewConnection();
    void slotReadyRead();
    void slotAcceptError();
    void slotDestroyed();
private:
    Ui::Server *ui;
    QTcpServer * m_server;
    QTcpSocket * m_client;
};

#endif // SERVER_H

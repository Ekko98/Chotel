#include "server.h"
#include "ui_server.h"

Server::Server(QWidget *parent) :
    QWidget(parent),
    m_server(NULL),
    m_client(NULL),
    ui(new Ui::Server)
{
    setAttribute(Qt::WA_DeleteOnClose);
    ui->setupUi(this);
    // 创建套接字对象
    m_server = new QTcpServer(this);
    //设定监听模式
    m_server->listen(QHostAddress::Any,6666);//监听从任意地址（any）的请求
    //通过信号接收客户端请求
    connect(m_server,&QTcpServer::newConnection,this,
            &Server::slotNewConnection);
    connect(m_server,&QTcpServer::acceptError,this,&Server::slotAcceptError);
    connect(m_server,&QTcpServer::destroyed,this,&Server::slotDestroyed);


}

Server::~Server()
{
    m_server->close();
    m_server->deleteLater();
    delete ui;
}

void Server::slotNewConnection()
{
    //处理客户端请求连接
    m_client=m_server->nextPendingConnection();
    m_client->write("服务器连接成功!");
    list_client.append(m_client);

    //连接信号，接受客户端数据
    connect(m_client,&QTcpSocket::readyRead,
            this,&Server::slotReadyRead);
    connect(m_client, &QTcpSocket::disconnected, this, &Server::slot_Disconnected);
}

void Server::slotReadyRead()
{
    int number;
    for(int i = 0;i < list_client.length();i ++)
        {
           message=list_client.at(i)->readAll();
           if(!message.isEmpty()){
               number=i;
               QMessageBox::information(this,"Client Message",message);
               QJsonObject Request_Client=QJsonDocument::fromJson(message).object();

               ui->label_room1_roomtem->setText(Request_Client.value("temperature").toString());
               send_message("收到",number);
               break;
           }
        }
}

Server::send_message(QByteArray message,int number)
{
    list_client.at(number)->write(message);
}

void Server::slot_Disconnected()
{
    for(int i = 0;i < list_client.length();i ++){
        if(list_client.at(i)->state() != QAbstractSocket::ConnectedState)
        {
            list_client.at(i)->deleteLater();
            list_client.removeAt(i);
            ui->label_room1_roomtem->setText("");
            QMessageBox::information(this,"Client Message","空调关闭");
        }
    }
}


void Server::slotAcceptError()
{
    QMessageBox::information(this,"Error","This is a error!");
}

void Server::slotDestroyed()
{
    QMessageBox::information(this,"~","It disconnect!");
}

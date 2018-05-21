#include "server.h"
#include "ui_server.h"

Server::Server(QWidget *parent) :
    QWidget(parent),
    m_server(NULL),
    m_client(NULL),
    ui(new Ui::Server)
{
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
    delete ui;
}

void Server::slotNewConnection()
{
   {
        //处理客户端请求连接
        m_client=m_server->nextPendingConnection();
        m_client->write("服务器连接成功!");

        //连接信号，接受客户端数据
        connect(m_client,&QTcpSocket::readyRead,
                this,&Server::slotReadyRead);
    }
}

void Server::slotReadyRead()
{
    //接受数据
    QByteArray array=m_client->readAll();
    QMessageBox::information(this,"Client Message",array);
    //处理数据
    QJsonObject Request_Client=QJsonDocument::fromJson(array).object();
    ui->label_room1_roomtem->setText(Request_Client.value("temperature").toString());
}

void Server::slotAcceptError()
{
    QMessageBox::information(this,"Error","This is a error!");
}

void Server::slotDestroyed()
{
    QMessageBox::information(this,"~","It disconnect!");
}

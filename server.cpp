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
    m_server->listen(QHostAddress::LocalHost,6666);
    //通过信号接收客户端请求
    connect(m_server,&QTcpServer::newConnection,this,
            &Server::slotNewConnection);
}

Server::~Server()
{
    delete ui;
}

void Server::slotNewConnection()
{
    if(m_client==NULL){
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
}

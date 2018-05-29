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
    //ui->widget_room1_shade->hide();

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
    list_client.append(m_client);//添加至list

    //连接信号，接受客户端数据
    connect(m_client,&QTcpSocket::readyRead,
            this,&Server::slotReadyRead);
    connect(m_client, &QTcpSocket::disconnected, this, &Server::slot_Disconnected);
}

void Server::slotReadyRead()
{
    //接受数据
    QByteArray array=m_client->readAll();
    QMessageBox::information(this,"Client Message",array);
    //处理数据
    QJsonObject Request_Client=QJsonDocument::fromJson(array).object();
    ui->label_room1_roomtem->setText(Request_Client.value("temperature").toString());
    //输出数据
    room tmp;
    tmp.aircond_tem=QString::number(26);
    QString id=Request_Client.value("id").toString();
    tmp.cost=QString::number(0);
    tmp.fee=QString::number(0);
    tmp.mode=Request_Client.value("operator").toString();
    tmp.roomtem=QString::number(Request_Client.value("temperature").toDouble());
    tmp.speed=Request_Client.value("operator").toString();
    read(id,tmp);

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

Server::send_message(QByteArray message,int number)//发信息
{
    list_client.at(number)->write(message);
}

void Server::slot_Disconnected()//空调关闭
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
    ui->widget_room1->hide();
    //ui->widget_room1_shade->show();
}


void Server::slotAcceptError()
{
    QMessageBox::information(this,"Error","This is a error!");
}

void Server::slotDestroyed()
{
    QMessageBox::information(this,"~","It disconnect!");
}

void Server::on_pushButton_clicked()
{

}

void Server::read(QString id,room tmp){

    bool speed_flag=true;
    bool mode_flag=true;
    if(tmp.speed=="H") tmp.speed="高速";
    else if(tmp.speed=="M") tmp.speed="中速";
    else if(tmp.speed=="L") tmp.speed="低速";
    else speed_flag=false;

    if(tmp.mode=="W") tmp.mode="制热";
    else if(tmp.mode=="C") tmp.mode="制冷";
    else mode_flag=false;
    //ip
    int ip=id.toInt();
    switch(ip){
        case 1:
            ui->label_room1_roomtem->setText(tmp.roomtem);
            ui->lineEdit_room1_aircond_tem->setText(tmp.aircond_tem);
            ui->lineEdit_room1_cost->setText(tmp.cost);
            ui->lineEdit_room1_fee->setText(tmp.fee);
            if(speed_flag)
                ui->lineEdit_room1_speed->setText(tmp.speed);
            if(mode_flag)
                ui->lineEdit_room1_mode->setText(tmp.mode);
        break;
        case 2:break;
        case 3:break;
        case 4:break;
        case 5:break;
        defalut:break;
    }
}

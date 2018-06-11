#include "server.h"
#include "ui_server.h"


QMap<QString,struct room> inf;
QList<Servered *> se;//6
QList<Scheduled *> sc;//20

Server::Server(QWidget *parent) :
    QWidget(parent),
    m_server(NULL),
    m_client(NULL),
    ui(new Ui::Server)
{
    standard=26;
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
    //QByteArray array=m_client->readAll();
    //QMessageBox::information(this,"Client Message",array);
    //处理数据
    //QJsonObject Request_Client=QJsonDocument::fromJson(array).object();
    //ui->label_room1_roomtem->setText(Request_Client.value("temperature").toString());
    //输出数据
    //read(id,tmp);

    int number;
    for(int i = 0;i < list_client.length();i ++)
        {
           message=list_client.at(i)->readAll();
           if(!message.isEmpty()){
               number=i;
               //QMessageBox::information(this,"Client Message",message);
               QJsonObject Request_Client=QJsonDocument::fromJson(message).object();
               struct room tmp;
               QString id;
               id=Request_Client.value("id").toString();
               QMap<QString, struct room>::iterator mi;
               //ui->label_room1_roomtem->setText(Request_Client.value("temperature").toString());
               QString temp=Request_Client.value("operator").toString();
               if(temp=="O"){
                   tmp.aircond_tem=standard;
                   tmp.fee=0;
                   tmp.state=zhileng;
                   tmp.roomtem=Request_Client.value("temperature").toString().toFloat();
                   //qDebug()<<Request_Client.value("temperature");
                   tmp.gear=mid;
                   inf.insert(id,tmp);
                   if(se.size()<6){
                       Servered *temp_Se=new Servered();
                       temp_Se->id=id;
                       se.append(temp_Se);
                       temp_Se->start();
                   }
                   else{
                       Scheduled *temp_Sc=new Scheduled();
                       temp_Sc->id=id;
                       sc.append(temp_Sc);
                       temp_Sc->start();

                   }
               }
               if(temp=="H"){
                   mi=inf.find(id);
                   mi->gear=high;
                   if(se.size()<6){
                       Servered *temp_Se=new Servered();
                       temp_Se->id=id;
                       se.append(temp_Se);
                       temp_Se->start();
                   }
                   else{
                       se.at(0)->end();
                       Scheduled *temp_Sc=new Scheduled();
                       temp_Sc->id=se.at(0)->id;
                       se.removeAt(0);
                       sc.append(temp_Sc);
                       Servered *temp_Se=new Servered();
                       temp_Se->id=id;
                       se.append(temp_Se);
                       temp_Se->start();

                   }
               }
               if(temp=="M"){
                   mi=inf.find(id);
                   mi->gear=mid;
                   if(se.size()<6){
                       Servered *temp_Se=new Servered();
                       temp_Se->id=id;
                       se.append(temp_Se);
                       temp_Se->start();
                   }
                   else{
                       Scheduled *temp_Sc=new Scheduled();
                       temp_Sc->id=id;
                       sc.append(temp_Sc);
                       temp_Sc->start();

                   }
               }
               if(temp=="L"){
                   mi=inf.find(id);
                   mi->gear=low;
                   if(se.size()<6){
                       Servered *temp_Se=new Servered();
                       temp_Se->id=id;
                       se.append(temp_Se);
                       temp_Se->start();
                   }
                   else{
                       Scheduled *temp_Sc=new Scheduled();
                       temp_Sc->id=id;
                       sc.append(temp_Sc);
                       temp_Sc->start();

                   }
               }
               if(temp=="S"){
                   mi=inf.find(id);
                   mi->state=clo;
               }
               if(temp=="U"){
                   send_message(number,id);
               }
           }
        }
}

void Server::send_message(int number,QString id)//发更新信息
{
    QJsonObject request_On_Obj;
    QMap<QString, struct room>::iterator p;
    p=inf.find(id);
    if(p!=inf.end()){
        request_On_Obj.insert("state",p.value().state);
        request_On_Obj.insert("temperature",QString::number(p.value().roomtem));
        request_On_Obj.insert("gear",p.value().gear);
        request_On_Obj.insert("fee",QString::number(p.value().fee));
        QJsonDocument request_On_Doc;
        request_On_Doc.setObject(request_On_Obj);
        QByteArray request_On_ByteArray=request_On_Doc.toJson(QJsonDocument::Compact);
        list_client.at(number)->write(request_On_ByteArray);
    }
}

void Server::control(QJsonObject Request)
{
//    QString id=Request.value("id").toString();
//    QString temp=Request.value("operator").toString();
//    if(se.size()<6){
//        // 服务队列不满
//        Servered temp_Se;
//        temp_Se.id=id;



//    }
}

void Server::slot_Disconnected()//退房
{
    for(int i = 0;i < list_client.length();i ++){
        if(list_client.at(i)->state() != QAbstractSocket::ConnectedState)
        {
            list_client.at(i)->deleteLater();
            list_client.removeAt(i);
            //ui->label_room1_roomtem->setText("");
            QMessageBox::information(this,"Client Message","退房");
        }
    }
    //ui->widget_room1->hide();
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

//void Server::read(QString id,room tmp){

//    bool speed_flag=true;
//    bool mode_flag=true;
//    if(tmp.speed=="H") tmp.speed="高速";
//    else if(tmp.speed=="M") tmp.speed="中速";
//    else if(tmp.speed=="L") tmp.speed="低速";
//    else speed_flag=false;

//    if(tmp.mode=="W") tmp.mode="制热";
//    else if(tmp.mode=="C") tmp.mode="制冷";
//    else mode_flag=false;
//    //ip
//    int ip=id.toInt();
//    switch(ip){
//        case 1:
//            ui->label_room1_roomtem->setText(tmp.roomtem);
//            ui->lineEdit_room1_aircond_tem->setText(tmp.aircond_tem);
//            ui->lineEdit_room1_cost->setText(tmp.cost);
//            ui->lineEdit_room1_fee->setText(tmp.fee);
//            if(speed_flag)
//                ui->lineEdit_room1_speed->setText(tmp.speed);
//            if(mode_flag)
//                ui->lineEdit_room1_mode->setText(tmp.mode);
//        break;
//        case 2:break;
//        case 3:break;
//        case 4:break;
//        case 5:break;
//        defalut:break;
//    }
//}

void Server::bill(){

}

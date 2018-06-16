#include "server.h"
#include "ui_server.h"
#include <QHBoxLayout>
#include <QVBoxLayout>
#include "singleuser.h"

singleuser *s_uer[100];
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
    init_db();

}




Server::~Server()
{
    database.close();
    m_server->close();
    m_server->deleteLater();
    delete ui;
}

void Server::slotNewConnection()
{
    //处理客户端请求连接
    m_client=m_server->nextPendingConnection();
    //m_client->write("服务器连接成功!");
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
            bool Inse=false;
            bool Insc=false;
            if(temp!="U"){
                insert_bill(id,temp);
            }
            if(temp=="O"){
                //maybe some bug
                if(inf.find(id)==inf.end()){
                    tmp.aircond_tem=standard;
                    qDebug()<<tmp.aircond_tem;
                    tmp.fee=0;
                    tmp.state=zhileng;
                    tmp.roomtem=Request_Client.value("temperature").toString().toFloat();
                    //qDebug()<<Request_Client.value("temperature");
                    tmp.gear=mid;
                    inf.insert(id,tmp);
                    addone(id);
                    if(se.size()<que_max){
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
                else{
                    inf.find(id).value().state=zhileng;
                    inf.find(id).value().roomtem=Request_Client.value("temperature").toString().toFloat();
                    if(se.size()<que_max){
                        Servered *temp_Se=new Servered();
                        temp_Se->id=id;
                        se.append(temp_Se);
                        temp_Se->start();
                    }
                    else{
                        if(inf.find(id).value().gear!=high){
                            Scheduled *temp_Sc=new Scheduled();
                            temp_Sc->id=id;
                            sc.append(temp_Sc);
                            temp_Sc->start();
                        }
                        else{

                            int M=0;
                            for(int m=0;m<se.size();m++){
                                if(se.at(m)->time>se.at(M)->time){
                                    M=m;
                                }
                            }
                            se.at(M)->end();
                            Scheduled *temp_Sc=new Scheduled();
                            temp_Sc->id=se.at(M)->id;
                            temp_Sc->start();
                            se.removeAt(M);
                            sc.append(temp_Sc);

                            Servered *temp_Se=new Servered();
                            temp_Se->id=id;
                            se.append(temp_Se);
                            temp_Se->start();
                        }

                    }
                    send_message(number,id);
                }
            }
            if(temp=="W"){
                inf.find(id).value().state=zhire;
            }
            if(temp=="C"){
                inf.find(id).value().state=zhileng;
            }
            if(temp=="H"){
                int k=0;
                for(int i=0;i<se.size();i++){
                    if(se.at(i)->id==id){
                        Inse=true;
                        break;
                    }
                }
                for(;k<sc.size();k++){
                    if(sc.at(k)->id==id){
                        Insc=true;
                        break;
                    }
                }
                mi=inf.find(id);

                if(Inse){
                    //                       Servered *temp_Se=new Servered();
                    //                       temp_Se->id=id;
                    //                       se.append(temp_Se);
                    //                       temp_Se->start();
                }
                else if(Insc&&mi->gear!=high){
                    sc.removeAt(k);
                    int M=0;
                    for(int m=0;m<se.size();m++){
                        if(se.at(m)->time>se.at(M)->time){
                            M=m;
                        }
                    }
                    se.at(M)->end();
                    Scheduled *temp_Sc=new Scheduled();
                    temp_Sc->id=se.at(M)->id;
                    temp_Sc->start();
                    se.removeAt(M);
                    sc.append(temp_Sc);

                    Servered *temp_Se=new Servered();
                    temp_Se->id=id;
                    se.append(temp_Se);
                    temp_Se->start();

                }
                mi->gear=high;
            }
            if(temp=="M"){
                mi=inf.find(id);
                mi->gear=mid;
            }
            if(temp=="L"){
                mi=inf.find(id);
                mi->gear=low;
            }
            if(temp=="S"){
                mi=inf.find(id);
                mi->state=clo;
                for(int i=0;i<se.size();i++){
                    if(se.at(i)->id==id){
                        se.at(i)->end();
                        se.removeAt(i);
                        if(sc.size()!=0){
                            int Min=0;
                            for(int m=0;m<se.size();m++){
                                if(se.at(m)->time<se.at(Min)->time){
                                    Min=m;
                                }
                            }
                            sc.at(Min)->end();
                            Servered *temp_Se=new Servered();
                            temp_Se->id=sc.at(Min)->id;
                            sc.removeAt(Min);
                            se.append(temp_Se);
                            temp_Se->start();
                        }

                        break;
                    }
                }
                for(int k=0;k<sc.size();k++){
                    if(sc.at(k)->id==id){
                        sc.at(k)->end();
                        sc.removeAt(k);
                        break;
                    }
                }

            }
            if(temp=="T"){
                inf.find(id).value().aircond_tem=
                        Request_Client.value("temperature").toString().toFloat();

            }
            if(temp=="U"){
                if(inf.find(id).value().state=="X"){
                    qDebug()<<"Huiwen";
                    for(int k=0;k<se.size();k++){
                        if(se.at(k)->id==id){
                            se.at(k)->end();
                            se.removeAt(k);
                            //temp_Sc->start();
                            if(sc.size()!=0){
                                int Min=0;
                                for(int m=0;m<sc.size();m++){
                                    if(se.at(m)->time<se.at(Min)->time){
                                        Min=m;
                                    }
                                }
                                sc.at(Min)->end();

                                Servered *temp_Se=new Servered();
                                temp_Se->id=sc.at(Min)->id;
                                sc.removeAt(Min);
                                se.append(temp_Se);
                                temp_Se->start();
                            }

                            break;
                        }

                    }
                    for(int i=0;i<sc.size();i++){
                        if(sc.at(i)->id==id){
                            sc.at(i)->end();
                            sc.removeAt(i);
                            break;
                        }
                    }

                }
                 send_message(number,id);
            }
        }
    }}
void Server::send_message(int number, QString id)//发更新信息
{
    QJsonObject request_On_Obj;
    QMap<QString, struct room>::iterator p;
    p=inf.find(id);
    if(p!=inf.end()){
        request_On_Obj.insert("state",p.value().state);
        request_On_Obj.insert("temperature",QString::number(p.value().roomtem));
        request_On_Obj.insert("gear",p.value().gear);
        request_On_Obj.insert("fee",QString::number(p.value().fee));
        request_On_Obj.insert("set_temperature",QString::number(p.value().aircond_tem));
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
void Server::init_db(){
    if (QSqlDatabase::contains("qt_sql_default_connection"))
    {
        database = QSqlDatabase::database("qt_sql_default_connection");
    }
    else
    {
        database = QSqlDatabase::addDatabase("QSQLITE");
        database.setDatabaseName("aircondition.db");
        database.setUserName("admin");
        database.setPassword("");
    }
    if (!database.open())
    {
        qDebug() << "Error: Failed to connect database." << database.lastError();
    }
    else
    {
        QSqlQuery sql_query;
        QString create_sql="drop table bill";
        sql_query.prepare(create_sql);
        if(!sql_query.exec())
        {
            qDebug() << "Error: Fail to drop table." << sql_query.lastError();
        }
        else
        {
            qDebug() << "Drop!";
        }

        create_sql = "create table bill ("
                     "time varchar(50), "
                     "id varchar(30), "
                     "operation varchar(20),"
                     "fee varchar(20)"
                     ")";
        sql_query.prepare(create_sql);
        if(!sql_query.exec())
        {
            qDebug() << "Error: Fail to create table." << sql_query.lastError();
        }
        else
        {
            qDebug() << "Table created!";
        }
    }
}

void Server::insert_bill(QString id,QString op){
    {
        QDateTime current_date_time =QDateTime::currentDateTime();
        QString current_date =current_date_time.toString("yyyy.MM.dd hh:mm:ss");
        QMap<QString, struct room>::iterator p;
        p=inf.find(id);

        QString insert_sql = "insert into bill values (?, ?, ?, ?)";
        QSqlQuery sql_query;
        sql_query.prepare(insert_sql);
        sql_query.addBindValue(current_date);
        sql_query.addBindValue(id);
        sql_query.addBindValue(op);
        sql_query.addBindValue(QString::number(p->fee));
        if(!sql_query.exec())
        {
            qDebug() << sql_query.lastError();
        }
        else
        {
            qDebug() << "inserted!";
        }
    }
}

void Server::generate_bill(QString id){
    QString select_sql = "select * from student where id=:id";\
    QSqlQuery sql_query;
    sql_query.prepare(select_sql);
    sql_query.bindValue(":id","\""+id+"\"");
    if(!sql_query.exec())
    {
        qDebug()<<sql_query.lastError();
    }
    else
    {
        while(sql_query.next())
        {
            QString ttime = sql_query.value(0).toString();
            QString tid = sql_query.value(1).toString();
            QString top = sql_query.value(2).toString();
            QString fee = sql_query.value(3).toString();
        }
    }
}


//显示账单,有待改进
void Server::on_button_generatebill_1_clicked(){
    DSheet * d1=new DSheet();
    d1->show();

}


void Server::addone(QString id)
{
    int n=inf.size();
    if(id!="null"){
        s_uer[n-1]=new singleuser(0,id);
    }
    int row=5;//行
    int col=3;//列

    QWidget *window=new QWidget;
    QHBoxLayout *h_layout = new QHBoxLayout[row];
    QVBoxLayout *v_layout = new QVBoxLayout;

    if(n<3)
      col=n;

    for(int j=0;j<row;j++)
    {
        if(j*col+1>n)
          break;
        for(int i=0;i<col;i++)
        {
            if(j*col+i+1>n)
                break;
            (&h_layout[j])->addWidget(s_uer[j*col+i]);
            QObject::connect(this,SIGNAL(sendData(room)),s_uer[j*col+i],SLOT(receiveData(room)));
            //QObject::connect(this,SIGNAL(add(QString)),s_uer[j*col+i],SLOT(adduser(QString)));
        }
        v_layout->addLayout(&h_layout[j]);
     }

    v_layout->addStretch(0);//设置layout不能被自动调整大小
    v_layout->addStretch(0);
    v_layout->addStretch(0);//layoutstretch里面的初始值为0,0,0，意思是三个控件的比例是1：1：1。
    window->setLayout(v_layout);
    window->setMinimumHeight(201*(n/3+1));
    window->setMinimumWidth(147*3);
    QSizePolicy policy=window->sizePolicy();
    policy.setHorizontalStretch(1);
    policy.setVerticalStretch(1);
    policy.setHorizontalPolicy(QSizePolicy::Minimum);
    policy.setVerticalPolicy(QSizePolicy::Minimum);
    window->setSizePolicy(policy);

    ui->scrollArea->setWidgetResizable(true);
    ui->scrollArea->setWidget(window);  //必须做的一步，将这个子窗口imageLabel装进scrollArea
    this->repaint();
    this->update();
}

void Server::deleteone(QString id)
{
    int n;
    for(int i=0;i<inf.size();i++){
        if(s_uer[i]->id==id){
            n=i;
            break;
        }
    }
    for(int i=n;i<inf.size();i++){
        s_uer[i]=s_uer[i+1];
    }
    delete s_uer[inf.size()];
    inf.remove(id);
    addone("null");
}
/*
void Server::on_text11_clicked()
{
    room update;
    update.fee=66;
    emit sendData(update);
}
*/


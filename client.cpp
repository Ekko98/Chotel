#include "client.h"
#include "ui_client.h"
extern QMap<QString,struct room> inf;
int room_set;
float room_air;
bool on_flag;


Client::Client(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Client)
{
    setAttribute(Qt::WA_DeleteOnClose);
    ui->setupUi(this);
    connect(ui->button_On_Off,&QPushButton::clicked,this,&Client::slotSendOnOffMsg);
    //connect(ui->button_speed_high,&QPushButton::clicked,this,&Client::slotSendotherMsg);
    //connect(ui->button_mode_hot,&QPushButton::clicked,this,&Client::slotSendotherMsg);
    timer = new QTimer(this);
    timer2= new QTimer(this);
    connect(timer2,&QTimer::timeout,this,&Client::huiwen);
    connect(timer,&QTimer::timeout,this,&Client::slot_send);
    on_flag=false;
    //room_set=26;
    //创建套接字
    m_client = new QTcpSocket(this);

    //连接服务器

    m_client->connectToHost("192.168.1.102",6666);

    //m_client->connectToHost("localhost",6666);
    // m_client->connectToHost("192.168.137.73",8080);
//ddlyt172.20.10.9

    //通过信号通信服务器
    connect(m_client, &QTcpSocket::readyRead,this, &Client::slotReadyRead);
    connect(m_client, &QTcpSocket::disconnected, this, &Client::slotDisconnected);
    //m_filterexp->setContentsMargins(5, 0, 3, 1);
    ui->input_t_room->setText("15");
}



Client::~Client()
{
    m_client->disconnectFromHost();
    m_client->deleteLater();
    delete ui;
}


void Client::initRoom(string id)
{
    ui->label_roomid->setText(QString::fromStdString(id));

}

void Client::slotReadyRead()
{
    QByteArray array=m_client->readAll();
    //QMessageBox::information(this,"Server Message",array);
    array.remove(array.size()-1,1);
    //qDebug()<<array;
    QJsonObject Request_Client=QJsonDocument::fromJson(array).object();
    ui->input_t_room->setText(QString::number(
                                  Request_Client.value("temperature").toString().toFloat(),'f',2));
    //ui->input_mode->setText(Request_Client.value("state").toString());
    ui->input_fee->setText(Request_Client.value("fee").toString());
    ui->input_t_aircondi->setText(Request_Client.value("set_temperature").toString());
    //ui->input_speed->setText(Request_Client.value("gear").toString());
    QString temp=Request_Client.value("state").toString();
    if(temp=="S"){
        ui->input_mode->setText("已关机");
        timer->stop();

    }
    else if(temp=="W"){
        ui->input_mode->setText("制热");
        cflag=0;
        //qDebug()<<"meicuo";
    }
    else if(temp=="C"){
        ui->input_mode->setText("制冷");
        cflag=1;
    }
    else if(temp=="A"){
        ui->input_mode->setText("自动");
    }
    else if(temp=="X"){
        ui->input_mode->setText("回温中");
        timer->stop();
        timer2->start(1000);
    }
    temp=Request_Client.value("gear").toString();
    if(temp=="H"){
        ui->input_speed->setText("高速");
    }
    else if(temp=="M"){
        ui->input_speed->setText("中速");
    }
    else if(temp=="L"){
        ui->input_speed->setText("低速");
    }

    //ui->label_roomid->setText(array);
}

void Client::slotSendOnOffMsg()
{
    //send open request
    if(ui->button_On_Off->text()=="开机"){
        ui->button_On_Off->setText("关机");
        //m_client->write("hello,I am");
        on_flag=true;

        write_obj("O");
        QJsonDocument request_On_Doc;
        request_On_Doc.setObject(request_On_Obj);
        QByteArray request_On_ByteArray=request_On_Doc.toJson(QJsonDocument::Compact);
        request_On_ByteArray+='\n';
        m_client->write(request_On_ByteArray);
        timer2->stop();

        //should init the infomation
        //ui->input_fee->setText("0.0");
        //ui->input_speed->setText("低速");
        //ui->input_mode->setText("制冷");
        //ui->input_fee->setText(request_On_Obj.value("temperature").toString());


        timer->start(stall); // 5秒单触发定时器
    }
    else{
        on_flag=false;
        //clear the screen
        write_obj("S");
        //m_client->write("bye");
        //ui->input_t_aircondi->setText("");
        //ui->input_fee->setText("");
        //ui->input_speed->setText("");
        //ui->input_mode->setText("");
        //ui->input_t_room->setText("");
        //m_client->disconnectFromHost();//断开连接
        QJsonDocument request_On_Doc;
        request_On_Doc.setObject(request_On_Obj);
        QByteArray request_On_ByteArray=request_On_Doc.toJson(QJsonDocument::Compact);
        request_On_ByteArray+='\n';
        m_client->write(request_On_ByteArray);
        //timer->stop();
        ui->button_On_Off->setText("开机");

    }
}

//void Client::slotSendotherMsg()
//{
//    if(on_flag==true){
//        write_obj("H");
//        ui->input_mode->setText("制冷");
//   }

//    QJsonDocument request_On_Doc;
//    request_On_Doc.setObject(request_On_Obj);
//    QByteArray request_On_ByteArray=request_On_Doc.toJson(QJsonDocument::Compact);
//    if(m_client->state()==QAbstractSocket::ConnectingState || m_client->state()==QAbstractSocket::ConnectedState)
//        m_client->write(request_On_ByteArray);
//}

void Client::slotDisconnected()
{
    QMessageBox::information(this,"Server Message","断开连接");

}

// for open room's air-conditor
void Client::on_button_On_Off_clicked()
{

}

//for request a cold mode
void Client::on_button_mode_cold_clicked()
{
//    if(on_flag==true&&ui->input_mode->text()!="制冷"){
//        write_obj("C");
//        ui->input_mode->setText("制冷");
//        QJsonDocument request_On_Doc;
//        request_On_Doc.setObject(request_On_Obj);
//        QByteArray request_On_ByteArray=request_On_Doc.toJson(QJsonDocument::Compact);
//        m_client->write(request_On_ByteArray);
//    }
//    else{
//        QMessageBox::information(this,"提示！","谢谢");
//    }
    QMessageBox::information(this,"提示！","已经在制冷");

}

//for request a hot mode
void Client::on_button_mode_hot_clicked()
{
//    if(on_flag==true&&ui->input_mode->text()!="制热"){
//        write_obj("W");
//        ui->input_mode->setText("制热");
//        QJsonDocument request_On_Doc;
//        request_On_Doc.setObject(request_On_Obj);
//        QByteArray request_On_ByteArray=request_On_Doc.toJson(QJsonDocument::Compact);
//        m_client->write(request_On_ByteArray);

//    }
//    else{
//        QMessageBox::information(this,"别瞎**乱动！","谢谢");
//    }
    QMessageBox::information(this,"提示！","无法更改");
}


//for request a low speed
void Client::on_button_speed_low_clicked()
{
    if(on_flag==true&&ui->input_speed->text()!="低速"){
        write_obj("L");
        ui->input_speed->setText("低速");
        QJsonDocument request_On_Doc;
        request_On_Doc.setObject(request_On_Obj);
        QByteArray request_On_ByteArray=request_On_Doc.toJson(QJsonDocument::Compact);
        request_On_ByteArray+='\n';
        m_client->write(request_On_ByteArray);
    }
    else{
        QMessageBox::information(this,"提示！","无法生效");
    }
}


//for request a middle speed
void Client::on_button_speed_middle_clicked()
{
    if(on_flag==true&&ui->input_speed->text()!="中速"){
        write_obj("M");
        ui->input_speed->setText("中速");
        QJsonDocument request_On_Doc;
        request_On_Doc.setObject(request_On_Obj);
        QByteArray request_On_ByteArray=request_On_Doc.toJson(QJsonDocument::Compact);
        request_On_ByteArray+='\n';
        m_client->write(request_On_ByteArray);
    }
    else{
        QMessageBox::information(this,"提示！","无法生效");
    }
}


//for request a high speed
void Client::on_button_speed_high_clicked()
{
    if(on_flag==true&&ui->input_speed->text()!="高速"){
        write_obj("H");
        ui->input_speed->setText("高速");
        QJsonDocument request_On_Doc;
        request_On_Doc.setObject(request_On_Obj);
        QByteArray request_On_ByteArray=request_On_Doc.toJson(QJsonDocument::Compact);
        request_On_ByteArray+='\n';
        m_client->write(request_On_ByteArray);
    }
    else{
        QMessageBox::information(this,"提示！","无法生效");
    }
}


//for request up 1℃
//void Client::on_button_up_1_clicked()
//{
//    if(on_flag==true){
//        if(room_set<30){
//            QString out;
//            room_set++;
//            out=QString::number(room_set);
//            ui->input_t_aircondi->setText(out);
//            write_obj("I");
//        }
//    }
//}


//for request down 1℃

//void Client::on_button_down_1_clicked()
//{
//    if(on_flag==true){
//        if(room_set>16){
//            QString out;
//            room_set--;
//            out=QString::number(room_set);
//            ui->input_t_aircondi->setText(out);
//            write_obj("D");
//        }
//    }
//}

//checkout
void Client::on_button_checkout_clicked()
{
    this->close();
    //load out
}

void Client::write_obj(QString my_operator){
    request_On_Obj.remove("id");
    request_On_Obj.remove("operator");
    request_On_Obj.remove("temperature");
    request_On_Obj.insert("id",ui->label_roomid->text());
    request_On_Obj.insert("operator",my_operator);
    if(my_operator=="O")
        request_On_Obj.insert("temperature",ui->input_t_room->text());
    else if(my_operator=="T")
        request_On_Obj.insert("temperature",ui->input_change_aircondi->text());
    else
        request_On_Obj.insert("temperature",ui->input_t_aircondi->text());

}

void Client::slot_send(){
    write_obj("U");
    QJsonDocument request_On_Doc;
    request_On_Doc.setObject(request_On_Obj);
    QByteArray request_On_ByteArray=request_On_Doc.toJson(QJsonDocument::Compact);
    request_On_ByteArray+='\n';
    m_client->write(request_On_ByteArray);
}


//something wrong
void Client::huiwen(){
    float num = ui->input_t_room->text().toFloat();//fangian
    float num1 = ui->input_t_aircondi->text().toFloat();
    //默认制冷时候升温
    if(cflag==1){
        num+=(float)1;
    }
    else if(cflag==0){
        num-=(float)1;
    }
    //qDebug()<<num;
    ui->input_t_room->setText(QString::number(num));
    //inf.find(ui->label_roomid->text()).value().roomtem=num;
    //ui->input_t_aircondi->setText(num1);

    if( (cflag==0 && num-num1<=-2) || (cflag==1 && num-num1>=2) ){
        timer2->stop();

        if(ui->button_On_Off->text()=="关机"){

            write_obj("O");//
            QJsonDocument request_On_Doc;
            request_On_Doc.setObject(request_On_Obj);
            QByteArray request_On_ByteArray=request_On_Doc.toJson(QJsonDocument::Compact);
            request_On_ByteArray+='\n';
            m_client->write(request_On_ByteArray);
            timer->start(stall);
        }
        else{
            ui->input_mode->setText("已关机");
        }

    }
}

void Client::on_Button_changeT_clicked()
{
    QString changeT=ui->input_change_aircondi->text();
    if(changeT!=""){
        write_obj("T");
        QJsonDocument request_On_Doc;
        request_On_Doc.setObject(request_On_Obj);
        QByteArray request_On_ByteArray=request_On_Doc.toJson(QJsonDocument::Compact);
        request_On_ByteArray+='\n';
        m_client->write(request_On_ByteArray);
    }
}

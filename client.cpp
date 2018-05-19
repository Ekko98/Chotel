#include "client.h"
#include "ui_client.h"

Client::Client(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Client)
{
    ui->setupUi(this);
    //创建套接字
    m_client = new QTcpSocket(this);

   //连接服务器

    m_client->connectToHost(QHostAddress::LocalHost,6666);

    //通过信号通信服务器
    connect(m_client, &QTcpSocket::readyRead,

    this, &Client::slotReadyRead);
    connect(ui->button_On_Off,&QPushButton::clicked,this,&Client::slotSendOnOffMsg);
}



Client::~Client()
{
    Client::disconnect();
    delete ui;
}


Client::initRoom(string id)
{
    ui->label_roomid->setText(QString::fromStdString(id));
    ui->input_t_room->setText("30.6");

}

void Client::slotReadyRead()
{
    QByteArray array=m_client->readAll();
    QMessageBox::information(this,"Server Message",array);
}

void Client::slotSendOnOffMsg()
{
    //send open request
    if(ui->button_On_Off->text()=="开机"){

        //m_client->write("hello,I am");
        QJsonObject request_On_Obj;
        request_On_Obj.insert("id",ui->label_roomid->text());
        request_On_Obj.insert("operator","O");
        request_On_Obj.insert("temperature",ui->input_t_room->text());
        ui->button_On_Off->setText("关机");
        QJsonDocument request_On_Doc;
        request_On_Doc.setObject(request_On_Obj);
        QByteArray request_On_ByteArray=request_On_Doc.toJson(QJsonDocument::Compact);
        m_client->write(request_On_ByteArray);
        //should init the infomation
    }
    else{
        ui->button_On_Off->setText("开机");
        //clear the screen
        m_client->write("bye");
    }
}

// for open room's air-conditor
void Client::on_button_On_Off_clicked()
{


}

//for request a cold mode
void Client::on_button_mode_cold_clicked()
{

}

//for request a hot mode
void Client::on_button_mode_hot_clicked()
{

}


//for request a low speed
void Client::on_button_speed_low_clicked()
{

}


//for request a middle speed
void Client::on_button_speed_middle_clicked()
{

}


//for request a high speed
void Client::on_button_speed_high_clicked()
{

}


//for request up 1℃
void Client::on_button_up_1_clicked()
{

}


//for request down 1℃
void Client::on_button_down_1_clicked()
{

}

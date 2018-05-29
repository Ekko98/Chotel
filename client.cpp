#include "client.h"
#include "ui_client.h"
int room_set=26;
float room_air;
QJsonObject request_On_Obj;
bool on_flag=false;
Client::Client(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Client)
{
    setAttribute(Qt::WA_DeleteOnClose);
    ui->setupUi(this);
    connect(ui->button_On_Off,&QPushButton::clicked,this,&Client::slotSendOnOffMsg);
    //connect(ui->button_speed_high,&QPushButton::clicked,this,&Client::slotSendotherMsg);
    connect(ui->button_mode_hot,&QPushButton::clicked,this,&Client::slotSendotherMsg);
}



Client::~Client()
{
    m_client->disconnectFromHost();
    m_client->deleteLater();
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
    //ui->label_roomid->setText(array);

}

void Client::slotSendOnOffMsg()
{
    //send open request
    if(ui->button_On_Off->text()=="开机"){
        ui->button_On_Off->setText("关机");
        //创建套接字
        m_client = new QTcpSocket(this);
       //连接服务器
        m_client->connectToHost("127.0.0.1",6666);
        qDebug()<<m_client->errorString();

        //通过信号通信服务器
        connect(m_client, &QTcpSocket::readyRead,this, &Client::slotReadyRead);
        connect(m_client, &QTcpSocket::disconnected, this, &Client::slotDisconnected);

        connect(ui->button_speed_high,&QPushButton::clicked,this,&Client::slotSendotherMsg);
        connect(ui->button_speed_middle,&QPushButton::clicked,this,&Client::slotSendotherMsg);
         connect(ui->button_speed_low,&QPushButton::clicked,this,&Client::slotSendotherMsg);

        //m_client->write("hello,I am");
        on_flag=true;
        write_obj("O");
        QJsonDocument request_On_Doc;
        request_On_Doc.setObject(request_On_Obj);
        QByteArray request_On_ByteArray=request_On_Doc.toJson(QJsonDocument::Compact);
        if(m_client->state()==QAbstractSocket::ConnectingState)
        {
            m_client->write(request_On_ByteArray);
        }

        //should init the infomation
        ui->input_t_aircondi->setText("26");
        ui->input_fee->setText("0.0");
        ui->input_speed->setText("低速");
        ui->input_mode->setText("制冷");
        //ui->input_fee->setText(request_On_Obj.value("temperature").toString());
    }
    else{
        on_flag=false;
        ui->button_On_Off->setText("开机");
        //clear the screen
        //write_obj("S");
        m_client->write("bye");
        ui->input_t_aircondi->setText("");
        ui->input_fee->setText("");
        ui->input_speed->setText("");
        ui->input_mode->setText("");
        m_client->disconnectFromHost();//断开连接

    }
}

void Client::slotSendotherMsg()
{
//    if(on_flag==true){
//        write_obj("H");
//        ui->input_mode->setText("制冷");
//    }

    QJsonDocument request_On_Doc;
    request_On_Doc.setObject(request_On_Obj);
    QByteArray request_On_ByteArray=request_On_Doc.toJson(QJsonDocument::Compact);
    if(m_client->state()==QAbstractSocket::ConnectedState)
        m_client->write(request_On_ByteArray);
}

void Client::slotDisconnected()
{
    QMessageBox::information(this,"Server Message","断开连接");
    ui->button_On_Off->setText("开机");
}

// for open room's air-conditor
void Client::on_button_On_Off_clicked()
{

}

//for request a cold mode
void Client::on_button_mode_cold_clicked()
{
    if(on_flag==true){
        write_obj("C");
        ui->input_mode->setText("制冷");
    }

}

//for request a hot mode
void Client::on_button_mode_hot_clicked()
{
    if(on_flag==true){
        write_obj("W");
        ui->input_mode->setText("制热");
    }
}


//for request a low speed
void Client::on_button_speed_low_clicked()
{
    if(on_flag==true){
        write_obj("L");
        ui->input_speed->setText("低速");
    }
}


//for request a middle speed
void Client::on_button_speed_middle_clicked()
{
    if(on_flag==true){
        write_obj("M");
        ui->input_speed->setText("中速");
    }
}


//for request a high speed
void Client::on_button_speed_high_clicked()
{
//    if(on_flag==true){
//        write_obj("H");
//        ui->input_speed->setText("高速");
//    }
}


//for request up 1℃
void Client::on_button_up_1_clicked()
{
    if(on_flag==true){
        if(room_set<30){
            QString out;
            room_set++;
            out=QString::number(room_set);
            ui->input_t_aircondi->setText(out);
            write_obj("I");
        }
    }
}


//for request down 1℃
void Client::on_button_down_1_clicked()
{
    if(on_flag==true){
        if(room_set>16){
            QString out;
            room_set--;
            out=QString::number(room_set);
            ui->input_t_aircondi->setText(out);
            write_obj("D");
        }
    }
}

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
    request_On_Obj.insert("temperature",ui->input_t_room->text());
}


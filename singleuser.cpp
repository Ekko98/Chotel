#include "singleuser.h"
#include "server.h"
#include "ui_singleuser.h"
#include <QDebug>
extern QList<Servered *> se;//6
extern QList<Scheduled *> sc;//20
extern Server * Admin;
extern QMap<QString,struct room> inf;
singleuser::singleuser(QWidget *parent,QString id) :
    QWidget(parent),
    ui(new Ui::singleuser)
{
    ui->setupUi(this);
    this->id=id;
    ui->roomlabel->setText(id);
    timer=new QTimer();
    timer->start(1000);
    //timer->start(stall);
    connect(timer,&QTimer::timeout,this,&singleuser::Output);
    QObject::connect(this,SIGNAL(deletethis(QString)),Admin,SLOT(deleteone(QString)));
}

singleuser::~singleuser()
{
    delete ui;
}

void singleuser::Output()
{
    QMap<QString,struct room>::iterator p;
    p=inf.find(this->id);
    if(p==inf.end()){
        timer->stop();
        return;
    }
    room update=p.value();

    if(update.state=="S"){
        ui->mode->setText("已关机");

    }
    else if(update.state=="W"){
        ui->mode->setText("制热");
    }
    else if(update.state=="C"){
        ui->mode->setText("制冷");
    }
    else if(update.state=="A"){
        ui->mode->setText("自动");
    }
    else if(update.state=="X"){
        ui->mode->setText("回温中");
        //timer->stop();
        //timer2->start(1000);
    }

    if(update.gear=="H"){
        ui->speed->setText("高速");
    }
    else if(update.gear=="M"){
        ui->speed->setText("中速");
    }
    else if(update.gear=="L"){
        ui->speed->setText("低速");
    }
    QString str=QString::number(update.fee);
    ui->fee->setText(str);
    str=QString::number(update.aircond_tem);
    ui->aircond_tem->setText(str);
    str=QString::number(update.roomtem);
    ui->roomtem->setText(str);
    str=QString::number(update.time);
    ui->time->setText(str);

    for (int i = 0; i < se.size(); ++i) {
        if (se.at(i)->id ==this->id){
            ui->widget_room1->setStyleSheet("background:rgb(218, 255, 174)");//my_green
            return;
        }
    }
    for (int i = 0; i < sc.size(); ++i) {
        if (sc.at(i)->id ==this->id){
            ui->widget_room1->setStyleSheet("background:rgb(212, 255, 130)");//my_yellow
            return;
        }
    }
    ui->widget_room1->setStyleSheet("background:rgb(255, 53, 56)");//my_red
}


void singleuser::receiveData(room update){
     qDebug()<<"receiveed:"<<update.fee<<endl;
     Output();
     //ui->widget_room1->setBackgroundRole(QPalette::Shadow);//Light,Midlight,Dark,Mid,Shadow
     ui->widget_room1->setStyleSheet("background:rgb(255, 53, 56)");//my_red
     ui->widget_room1->setStyleSheet("background:rgb(212, 255, 130)");//my_yellow
     ui->widget_room1->setStyleSheet("background:rgb(218, 255, 174)");//my_green
}
/*
void singleuser::adduser(QString id){
     qDebug()<<"receiv---id:"<<id<<endl;
     if(this->id=="default"){
        this->id=id;
        ui->roomlabel->setText(id);
     }

*/

void singleuser::on_generatebill_1_clicked()
{

    if(ui->mode->text()=="已关机"){
        DSheet * d=new DSheet();
        d->generateDF(ui->roomlabel->text());
        d->show();
    }
    else{
        qDebug()<<"关机后重试";
    }
}



void singleuser::on_pushButton_clicked()
{
    if(inf.find(id).value().state=="S")
    {
        emit deletethis(this->id);
        inf.erase(inf.find(id));
        qDebug()<<id<<"已退房";

    }
    else{
         qDebug()<<id<<"无法强制退房";
        }
}

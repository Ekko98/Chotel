#include "client.h"
#include "ui_client.h"

Client::Client(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Client)
{
    ui->setupUi(this);
}

Client::~Client()
{
    delete ui;
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

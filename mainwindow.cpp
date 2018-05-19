#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

//for get a room id
void MainWindow::on_checkin_button_clicked()
{
 if(ui->input_name->text()!=QString("admin")){
     Client * NewRoom=new Client();
     NewRoom->show();
     NewRoom->initRoom(ui->input_name->text().toStdString());
 }
 else{
     Server * Admin=new Server();
     Admin->show();
 }

}

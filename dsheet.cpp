#include "dsheet.h"
#include "ui_dsheet.h"



extern QMap<QString,struct room> inf;

DSheet::DSheet(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::DSheet)
{
    ui->setupUi(this);
}

void DSheet::generateDF(QString id)
{
    ui->label_id->setText(id);
    ui->lineEdit_fee->setText(QString::number(inf.find(id).value().fee));

    QSqlQuery select_sql;
    //QString select_sql = "select * from student where id=:id";
    QSqlQuery sql_query;
    sql_query.prepare("select * from bill");
    //sql_query.bindValue(":id","\""+id+"\"");
    if(!sql_query.exec())
    {
        qDebug()<<sql_query.lastError();
    }
    else
    {
        int i=0;
        int first=0;
        while(sql_query.next())
        {
            if(sql_query.value(1).toString()==id){
                QString ttime = sql_query.value(0).toString();
                if(first==0){

                    ui->lineEdit_time->setText(ttime);
                }
                QString top = sql_query.value(2).toString();
                qDebug()<<top;
                QTableWidgetItem * item_time=new QTableWidgetItem(ttime);
                QTableWidgetItem * item_top=new QTableWidgetItem(top);
                //QTableWidgetItem *item_optime=new QTableWidgetItem(optime);
                ui->tableWidget_op->insertRow(i);
                ui->tableWidget_op->setItem(i,0,item_time);
                ui->tableWidget_op->setItem(i,1,item_top);
                i++;
                first++;
            }

        }
    }
}

DSheet::~DSheet()
{
    delete ui;
}

void DSheet::on_pushButton_pay_clicked()
{

}

#include "server.h"

extern QMap<QString,struct room> inf;


int Servered::work(){
    QMap<QString, struct room>::iterator mi;
    mi=inf.find(this->id);
    if(mi!=inf.end()){
        if(mi.value().state==zhileng){
            if(mi.value().gear=="L"){
                mi.value().roomtem-=rate_low;
                mi.value().fee+=fee_low;
            }
            if( mi.value().gear=="M"){
                mi.value().roomtem-=rate_mid;
                mi.value().fee+=fee_mid;
            }
            if( mi.value().gear=="H"){
                mi.value().roomtem-=rate_high;
                mi.value().fee+=fee_high;
            }
        }
        else if( mi.value().state==zhire){
            if( mi.value().gear=="L"){
                mi.value().roomtem+=rate_low;
                mi.value().fee+=fee_low;
            }
            if( mi.value().gear=="M"){
                mi.value().roomtem+=rate_mid;
                mi.value().fee+=fee_mid;
            }
            if( mi.value().gear=="H"){
                mi.value().roomtem+=rate_high;
                mi.value().fee+=fee_high;
            }
        }
        this->time++;
        this->op_time++;
        inf.find(this->id).value().op_time=this->op_time;
        inf.find(this->id).value().time=this->time;
        if( ((mi.value().roomtem-mi.value().aircond_tem)<=0 && mi.value().state==zhileng) ||
              ((mi.value().roomtem-mi.value().aircond_tem)>=0 && mi.value().state==zhire ) )
        {
            mi.value().state="X";
            return 1;
            //qDebug()<<"X";
        }
        else
            return 0;
    }
    else
        return 0 ;
}



void Servered::start(){
    this->time=inf.find(this->id).value().time;
    this->t= new QTimer();
    connect(this->t,SIGNAL(timeout()),this,SLOT(work()));
    this->t->start(1000);

}

void Servered::end(){
    t->stop();
    t->deleteLater();
    this->X=true;

}

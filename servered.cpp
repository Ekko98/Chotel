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
        if( mi.value().state==zhire){
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
        if( (mi.value().roomtem-mi.value().aircond_tem)<=0.01)
        {
            mi.value().state="X";
            this->end();
        }
        else return 0;
    }
    else return 0;
}



void Servered::start(){
    this->time=0;
    this->t= new QTimer();
    connect(this->t,SIGNAL(timeout()),this,SLOT(work()));
    this->t->start(1000);
}

void Servered::end(){
    t->stop();
    t->deleteLater();
    this->X=true;
}

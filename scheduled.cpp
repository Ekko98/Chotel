#include<server.h>
extern QMap<QString,struct room> inf;
extern QList<Servered *> se;//6
extern QList<Scheduled *> sc;//20
void Scheduled::start(){
    this->time=5;
    t= new QTimer();
    connect(this->t,SIGNAL(timeout()),this,SLOT(timer()));
    this->t->start(1000);
}

void Scheduled::end(){
    t->stop();
    t->deleteLater();
}

void Scheduled::timer(){
    this->time--;
    if(time==0){
        for(int i=0;i<sc.size();i++){
            if(sc.at(i)->id==this->id){
                sc.at(i)->end();
                //judge isNull
                se.at(0)->end();
                Servered *temp_Se=new Servered();
                temp_Se->id=this->id;

                se.append(temp_Se);
                temp_Se->start();
                Scheduled * temp_Sc=new Scheduled();
                temp_Sc->id=se.at(0)->id;
                se.removeAt(0);
                sc.removeAt(i);
                sc.append(temp_Sc);
                temp_Sc->start();
            }
        }
    }
}

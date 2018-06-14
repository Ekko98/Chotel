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
    if(this->time==0){
        for(int i=0;i<sc.size();i++){
            if(sc.at(i)->id==this->id){
                sc.at(i)->end();
                sc.removeAt(i);
                //judge isNull
                if(se.size()>0){
                    int M=0;
                    for(int m=0;m<se.size();m++){
                        if(se.at(m)->time>se.at(M)->time){
                            M=m;
                        }
                    }
                    se.at(M)->end();
                    Scheduled * temp_Sc=new Scheduled();
                    temp_Sc->id=se.at(M)->id;
                    se.removeAt(M);
                    sc.append(temp_Sc);
                    temp_Sc->start();
                    Servered *temp_Se=new Servered();
                    temp_Se->id=this->id;
                    se.append(temp_Se);
                    temp_Se->start();

                }
                else{
                    Servered *temp_Se=new Servered();
                    temp_Se->id=this->id;
                    se.append(temp_Se);
                    temp_Se->start();

                }
            }
        }
    }
}

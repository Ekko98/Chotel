#include<server.h>
extern QMap<QString,struct room> inf;
void Scheduled::start(){
    this->time=0;
    t= new QTimer();
    connect(this->t,SIGNAL(timeout()),this,SLOT(timer()));
    this->t->start(1000);
}

void Scheduled::end(){
    t->stop();
    t->deleteLater();
}

void Scheduled::timer(){
    this->time++;
}

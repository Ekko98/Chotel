#include "singleuser.h"
#include "ui_singleuser.h"

singleuser::singleuser(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::singleuser)
{
    ui->setupUi(this);
}

singleuser::~singleuser()
{
    delete ui;
}

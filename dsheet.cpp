#include "dsheet.h"
#include "ui_dsheet.h"

DSheet::DSheet(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::DSheet)
{
    ui->setupUi(this);
}

DSheet::~DSheet()
{
    delete ui;
}

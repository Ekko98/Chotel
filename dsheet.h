#ifndef DSHEET_H
#define DSHEET_H

#include <QWidget>
#include <heads.h>
#include <QtSql/QtSql>

namespace Ui {
class DSheet;
}

class DSheet : public QWidget
{
    Q_OBJECT

public:
    explicit DSheet(QWidget *parent = 0);
    void generateDF(QString id);
    ~DSheet();

private slots:
    void on_pushButton_pay_clicked();

private:
    Ui::DSheet *ui;
};

#endif // DSHEET_H

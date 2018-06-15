#ifndef SINGLEUSER_H
#define SINGLEUSER_H
#include <QWidget>
#include "heads.h"
//#include "server.h"
namespace Ui {
class singleuser;
}

class singleuser : public QWidget
{
    Q_OBJECT

public:
    explicit singleuser(QWidget *parent = 0,QString id="");
    ~singleuser();
    QString id;
    QTimer *timer;
    //void Output(room update);

public slots:
    void Output();
private slots:
    void receiveData(room update);
    //void adduser(QString id);

private:
    Ui::singleuser *ui;
};
//extern new_server * ns;
#endif // SINGLEUSER_H


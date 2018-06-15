#ifndef SINGLEUSER_H
#define SINGLEUSER_H

#include <QWidget>

namespace Ui {
class singleuser;
}

class singleuser : public QWidget
{
    Q_OBJECT

public:
    explicit singleuser(QWidget *parent = 0);
    ~singleuser();

private:
    Ui::singleuser *ui;
};

#endif // SINGLEUSER_H

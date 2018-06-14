#ifndef DSHEET_H
#define DSHEET_H

#include <QWidget>

namespace Ui {
class DSheet;
}

class DSheet : public QWidget
{
    Q_OBJECT

public:
    explicit DSheet(QWidget *parent = 0);
    ~DSheet();

private:
    Ui::DSheet *ui;
};

#endif // DSHEET_H

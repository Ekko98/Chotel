#ifndef CLIENT_H
#define CLIENT_H

#include <QWidget>

namespace Ui {
class Client;
}

class Client : public QWidget
{
    Q_OBJECT

public:
    explicit Client(QWidget *parent = 0);
    ~Client();

private slots:
    void on_button_On_Off_clicked();

    void on_button_mode_cold_clicked();

    void on_button_mode_hot_clicked();

    void on_button_speed_low_clicked();

    void on_button_speed_middle_clicked();

    void on_button_speed_high_clicked();

    void on_button_up_1_clicked();

    void on_button_down_1_clicked();

private:
    Ui::Client *ui;
};

#endif // CLIENT_H

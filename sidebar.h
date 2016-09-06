#ifndef SIDEBAR_H
#define SIDEBAR_H

#include <QWidget>

namespace Ui {
class SideBar;
}

class SideBar : public QWidget
{
    Q_OBJECT

public:
    explicit SideBar(QWidget *parent = 0);
    ~SideBar();


    bool getStatus(void)
    {
        return status;
    }

    void setStatus(bool isShow)
    {
        status = isShow;
    }

signals:
    void sidebar_request(bool);
    void connect_request();
    void rbtsettings_request();
    void visualize_request();

private slots:
    void on_sidebar_button_clicked();
    void on_connect_button_clicked();
    void on_rbtsettings_botton_clicked();

    void on_visualize_botton_clicked();

private:
    Ui::SideBar *ui;
    bool status;
};

#endif // SIDEBAR_H

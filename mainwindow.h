#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include "sidebar.h"
#include "robotsettings.h"
#include "tcpclient.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

    enum widgets_name {CONNECT_WGT, RBTSETTINGS_WGT, NUMS_WGT};

private slots:
    void show_sidebar(bool show);
    void show_connect();
    void show_rbtsettings();

    void on_button_connect_clicked();
    void download_table();

private:
    Ui::MainWindow *ui;

    QWidget* widgets[NUMS_WGT]; // except for sidebar
    SideBar* bar;
    QWidget* currentWidget;

    TcpClient cli;

    const int margin_x = 30;
};

#endif // MAINWINDOW_H

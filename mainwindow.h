#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTimer>

#include "sidebar.h"
#include "robotsettings.h"
#include "visualize.h"
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

    enum widgets_name {CONNECT_WGT, RBTSETTINGS_WGT, VISUALIZE_WGT, NUMS_WGT};

private slots:
    void show_sidebar(bool show);
    void show_connect();
    void show_rbtsettings();
    void show_visualize();
    /* @TODO hide sidebar when other widget active by user
     * */

    void on_button_connect_clicked();
    void download_table();
    void upload_table();
    void start_get_pose(bool isShow);
    void start_get_joints(bool isShow);
    void update_pose();
    void update_joints();

    void on_button_test_clicked();

private:
    Ui::MainWindow *ui;

    QWidget* widgets[NUMS_WGT]; // except for sidebar
    SideBar* bar;
    QWidget* currentWidget;

    QTimer* poseTimer;
    QTimer* jointsTimer;

    TcpClient cli;

    const int margin_x = 30;
};

#endif // MAINWINDOW_H

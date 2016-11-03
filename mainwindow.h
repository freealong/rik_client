#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTimer>

#include "mywidget.h"
#include "sidebar.h"
#include "robotsettings.h"
#include "visualize.h"
#include "taskassignment.h"
#include "tcpclient.h"
#include "udpclient.h"
#include "robotinfo.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

    enum widgets_name {CONNECT_WGT, RBTSETTINGS_WGT, VISUALIZE_WGT, TASKASSIGNMENT_WGT, NUMS_WGT};

private slots:
    void show_sidebar(bool show);
    void show_connect();
    void show_rbtsettings();
    void show_visualize();
    void show_task();

    void on_button_connect_clicked();
    void on_button_start_clicked();
    void download_robot_info();
    void upload_robot_info();
    void start_get_pose(bool isShow);
    void start_get_joints(bool isShow);
    void update_pose();
    void update_joints();
    void send_target(Eigen::VectorXf target, int mode);
    void send_mode(int mode);
    void send_increasing_mode(bool running, int mode, int num, float speed);

    void on_button_test_clicked();

    void on_button_motor_clicked();

    void on_button_print_clicked();

private:
    Ui::MainWindow *ui;

    MyWidget* widgets[NUMS_WGT]; // except for sidebar
    SideBar* bar;
    QWidget* currentWidget;

    QTimer* poseTimer;
    QTimer* jointsTimer;

    UdpClient ucli;
    TcpClient cli;
    RobotInfo robot;

    const int margin_x = 30;
};

#endif // MAINWINDOW_H

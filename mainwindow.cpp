#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QPropertyAnimation>
#include <QDebug>
#include <Eigen>

#include "pose.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    widgets[CONNECT_WGT] = ui->centralWidget;
    widgets[RBTSETTINGS_WGT] = new RobotSettings(this);
    widgets[VISUALIZE_WGT] = new Visualize(this);
    widgets[TASKASSIGNMENT_WGT] = new TaskAssignment(this);
    bar = new SideBar(this);
    poseTimer = new QTimer;
    jointsTimer = new QTimer;

    // set up background color;
    QPalette pal;
    pal.setColor(QPalette::Background, Qt::white);
    widgets[CONNECT_WGT]->setAutoFillBackground(true);
    widgets[CONNECT_WGT]->setPalette(pal);

    widgets[RBTSETTINGS_WGT]->setAutoFillBackground(true);
//    pal.setColor(QPalette::Background, Qt::yellow);
    widgets[RBTSETTINGS_WGT]->setPalette(pal);

    widgets[VISUALIZE_WGT]->setAutoFillBackground(true);
//    pal.setColor(QPalette::Background, Qt::yellow);
    widgets[VISUALIZE_WGT]->setPalette(pal);

    widgets[TASKASSIGNMENT_WGT]->setAutoFillBackground(true);
//    pal.setColor(QPalette::Background, Qt::yellow);
    widgets[TASKASSIGNMENT_WGT]->setPalette(pal);

//    bar->setAutoFillBackground(true);
//    pal.setColor(QPalette::Background, Qt::black);
    bar->setPalette(pal);

    // move widgets to initial place;
    currentWidget = widgets[CONNECT_WGT];

    widgets[RBTSETTINGS_WGT]->move(0, - widgets[RBTSETTINGS_WGT]->height());
    widgets[VISUALIZE_WGT]->move(0, - widgets[VISUALIZE_WGT]->height());
    widgets[TASKASSIGNMENT_WGT]->move(0, - widgets[TASKASSIGNMENT_WGT]->height());
    bar->move(margin_x - bar->width(), 0);
    bar->setStatus(false);

    connect(widgets[CONNECT_WGT], &MyWidget::getFocus, [=] () {show_sidebar(true);});
    connect(widgets[RBTSETTINGS_WGT], &MyWidget::getFocus, [=] () {show_sidebar(true);});
    connect(widgets[VISUALIZE_WGT], &MyWidget::getFocus, [=] () {show_sidebar(true);});
    connect(widgets[TASKASSIGNMENT_WGT], &MyWidget::getFocus, [=] () {show_sidebar(true);});
    // connect sidebar and other widgets
    connect(bar, &SideBar::connect_request, this, &MainWindow::show_connect);
    connect(bar, &SideBar::sidebar_request, this, &MainWindow::show_sidebar);
    connect(bar, &SideBar::rbtsettings_request, this, &MainWindow::show_rbtsettings);
    connect(bar, &SideBar::visualize_request, this, &MainWindow::show_visualize);
    connect(bar, &SideBar::task_request, this, &MainWindow::show_task);

    // connect this and robotsettings
    connect((RobotSettings*)widgets[RBTSETTINGS_WGT], &RobotSettings::download_request, this, &MainWindow::download_robot_info);
    connect((RobotSettings*)widgets[RBTSETTINGS_WGT], &RobotSettings::upload_request, this, &MainWindow::upload_robot_info);
    // connect this and visualize
    connect(poseTimer, SIGNAL(timeout()), this, SLOT(update_pose()));
    connect((Visualize*)widgets[VISUALIZE_WGT], &Visualize::pose_request, this, &MainWindow::start_get_pose);
    connect(jointsTimer, SIGNAL(timeout()), this, SLOT(update_joints()));
    connect((Visualize*)widgets[VISUALIZE_WGT], &Visualize::joints_request, this, &MainWindow::start_get_joints);
    // connect this and task
    connect((TaskAssignment*)widgets[TASKASSIGNMENT_WGT], &TaskAssignment::send_target_joints_request, this, &MainWindow::send_target_joints);
    connect((TaskAssignment*)widgets[TASKASSIGNMENT_WGT], &TaskAssignment::send_target_pose_request, this, &MainWindow::send_target_pose);
    connect((TaskAssignment*)widgets[TASKASSIGNMENT_WGT], &TaskAssignment::send_mode_request, this, &MainWindow::send_mode);
    connect((TaskAssignment*)widgets[TASKASSIGNMENT_WGT], &TaskAssignment::send_joints_request, this, &MainWindow::start_get_joints);
    connect((TaskAssignment*)widgets[TASKASSIGNMENT_WGT], &TaskAssignment::send_pose_request, this, &MainWindow::start_get_pose);
//    ui->tableWidget->setColumnCount(6);
//    ui->tableWidget->setRowCount(6);
//    QTableWidgetItem* item = new QTableWidgetItem;
//    item->setText("hello");
//    ui->tableWidget->setItem(1, 1, item);
}

MainWindow::~MainWindow()
{
    if (cli.is_connected())
        cli.disconnect_server();
    delete ui;
    delete bar;
    for (unsigned i = 0; i < NUMS_WGT; ++i)
        delete widgets[i];
}

void MainWindow::show_sidebar(bool show)
{
    QPropertyAnimation *anim = new QPropertyAnimation(bar, "geometry");
    anim->setDuration(300);
    anim->setStartValue(bar->geometry());
    anim->setEasingCurve(QEasingCurve::Linear);

    if (!show)
    {
//        bar->move(0, 0);
        bar->setStatus(true);
        anim->setEndValue(QRect(0, 0, bar->width(), bar->height()));
        anim->start();
        bar->setFocus();
    }
    else
    {
//        bar->move(margin_x - bar->width(), 0);
        anim->setEndValue(QRect(margin_x - bar->width(), 0, bar->width(), bar->height()));
        anim->start();
        bar->setStatus(false);
        currentWidget->setFocus();
    }
}

void MainWindow::show_connect()
{
    if (currentWidget == widgets[CONNECT_WGT])
        return;
    // else move current widget and show connect widget
    currentWidget->move(0, -currentWidget->height());
    widgets[CONNECT_WGT]->move(0, 0);
    currentWidget = widgets[CONNECT_WGT];

    bar->setFocus();
}


void MainWindow::show_rbtsettings()
{
    if (currentWidget == widgets[RBTSETTINGS_WGT])
        return;

    currentWidget->move(0, -currentWidget->height());
    widgets[RBTSETTINGS_WGT]->move(0, 0);
    currentWidget = widgets[RBTSETTINGS_WGT];

    bar->setFocus();
}

void MainWindow::show_visualize()
{
    if (currentWidget == widgets[VISUALIZE_WGT])
        return;
    currentWidget->move(0, -currentWidget->height());
    widgets[VISUALIZE_WGT]->move(0, 0);
    currentWidget = widgets[VISUALIZE_WGT];
    bar->setFocus();
}

void MainWindow::show_task()
{
    if (currentWidget == widgets[TASKASSIGNMENT_WGT])
        return;
    currentWidget->move(0, -currentWidget->height());
    widgets[TASKASSIGNMENT_WGT]->move(0, 0);
    currentWidget = widgets[TASKASSIGNMENT_WGT];
    bar->setFocus();
}

void MainWindow::on_button_connect_clicked()
{
    if(!cli.is_connected())
    {
        std::string ip = ui->lineEdit_ip->text().toStdString();
        int udp_port = ui->lineEdit_port->text().toInt();
        ucli.connect_server(ip.c_str(), udp_port);
        if(!ucli.isTcpListen)
        {
            ui->label->setText("Error on UDP connecting***");
            return;
        }
        ui->label->setText("Connecting...");
        int tcp_port = ucli.tcp_port;
        cli.connect_server(ip.c_str(), tcp_port);
        if(cli.is_connected())
        {
            ui->label->setText("Connected");
            ui->button_connect->setText("disconnect");
        }
        else
        {
            ui->label->setText("Error on TCP connecting***");
            ui->button_connect->setText("connect");
            qDebug() << "connected";
        }
    }
    else
    {
        cli.disconnect_server();
        cli = TcpClient();
        ui->label->setText("Disconnected!!!");
        ui->button_connect->setText("connect");
        qDebug() << "disconnected";
    }
}

void MainWindow::on_button_start_clicked()
{
    if (robot.is_initialized())
    {
        cli.release_robot();
        robot.clear();
        ui->button_start->setText("start");
        ui->label_robot->setText("Not started ...");
        ((RobotSettings*)widgets[RBTSETTINGS_WGT])->reset();
        ((Visualize*)widgets[VISUALIZE_WGT])->reset();
        ((TaskAssignment*)widgets[TASKASSIGNMENT_WGT])->reset();
    }
    else
    {
        if (cli.load_robot() >= 0)
        {
            download_robot_info();
            ui->label_robot->setText("SUCCESS on load robot");
            ui->button_start->setText("stop");
        }
        else
            ui->label_robot->setText("ERROR on load robot");
    }
}

void MainWindow::on_button_motor_clicked()
{
    std::string s = ui->button_motor->text().toStdString();
    if (cli.send_msg(s) < 0)
    {
        qDebug() << "ERROR on start/stop motor";
        return;
    }
    if (s == "start motor")
        ui->button_motor->setText("stop motor");
    else
        ui->button_motor->setText("start motor");
}

void MainWindow::download_robot_info()
{
    // @TODO: remove vlz to somewhere else
    RobotSettings* rbt = (RobotSettings*)widgets[RBTSETTINGS_WGT];
    dh_table t;
    joints_limits jl;
    if (cli.download_robot_info(t, jl) < 0)
    {
        qDebug() << "ERROR on download robot info";
        return;
    }
    robot.update_table(t);
    robot.update_joints_limits(jl);

    rbt->rbt_table = robot.get_dh_table();
    rbt->set_table(rbt->rbt_table);

    Visualize* vlz = (Visualize*)widgets[VISUALIZE_WGT];
    vlz->update_joints_widget(rbt->rbt_table);
}

void MainWindow::upload_robot_info()
{
    // @FIXME:THis is not done.
    RobotSettings* rbt = (RobotSettings*)widgets[RBTSETTINGS_WGT];
    rbt->get_table(rbt->rbt_table);
    joints_limits jl = robot.get_joints_limits();
    if (cli.upload_robot_info(rbt->rbt_table, jl) < 0)
    {
        qDebug() << "ERROR on upload robot info";
        return;
    }
    Visualize* vlz = (Visualize*)widgets[VISUALIZE_WGT];
    vlz->update_joints_widget(rbt->rbt_table);
}

void MainWindow::start_get_pose(bool isShow)
{
    if (isShow)
        poseTimer->start(100);
    else
        poseTimer->stop();
}

void MainWindow::start_get_joints(bool isShow)
{
    if (isShow)
        jointsTimer->start(100);
    else
        jointsTimer->stop();
}

void MainWindow::update_pose()
{
    // @FIXME:
    Eigen::VectorXf p;
    if (cli.get_current_pose(p) < 0)
    {
        qDebug() << "ERROR on update_pose";
        return;
    }
    robot.update_pose(p);

    Visualize* vlz = (Visualize*)widgets[VISUALIZE_WGT];
    vlz->update_pose(p);
    TaskAssignment* tsk = (TaskAssignment*)widgets[TASKASSIGNMENT_WGT];
    tsk->update_pose(p);
}

void MainWindow::update_joints()
{
    // @FIXME: same with update pose
    Eigen::VectorXf j;
    if (cli.get_current_joints(j) < 0)
    {
        qDebug() << "ERROR on update_joints";
        return;
    }
    robot.update_joints(j);

    Visualize* vlz = (Visualize*)widgets[VISUALIZE_WGT];
    vlz->update_joints(j);
    TaskAssignment* tsk = (TaskAssignment*)widgets[TASKASSIGNMENT_WGT];
    tsk->update_joints(j);
}

void MainWindow::send_target_joints(Eigen::VectorXf target)
{
    if (cli.send_target_joints(target) < 0)
    {
        qDebug() << "ERROR on send target joints";
        return;
    }
}

void MainWindow::send_target_pose(Eigen::VectorXf target)
{
    if (cli.send_target_pose(target) < 0)
    {
        qDebug() << "ERROR on send target pose";
        return;
    }
}

void MainWindow::send_mode(int mode)
{
    if (cli.send_mode(mode) < 0)
    {
        qDebug() << "ERROR on send mode";
        return;
    }
    if (mode == 0)
    {
        TaskAssignment* tsk = (TaskAssignment*)widgets[TASKASSIGNMENT_WGT];
        tsk->update_widget(robot.get_joints_num());
    }
}

void MainWindow::on_button_test_clicked()
{
    cli.test();
}

void MainWindow::on_button_print_clicked()
{
   robot.print_info();
}

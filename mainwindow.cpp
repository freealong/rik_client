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
    connect((RobotSettings*)widgets[RBTSETTINGS_WGT], &RobotSettings::download_request, this, &MainWindow::download_table);
    connect((RobotSettings*)widgets[RBTSETTINGS_WGT], &RobotSettings::upload_request, this, &MainWindow::upload_table);
    // connect this and visualize
    connect(poseTimer, SIGNAL(timeout()), this, SLOT(update_pose()));
    connect((Visualize*)widgets[VISUALIZE_WGT], &Visualize::pose_request, this, &MainWindow::start_get_pose);
    connect(jointsTimer, SIGNAL(timeout()), this, SLOT(update_joints()));
    connect((Visualize*)widgets[VISUALIZE_WGT], &Visualize::joints_request, this, &MainWindow::start_get_joints);
    // connect this and task
    connect((TaskAssignment*)widgets[TASKASSIGNMENT_WGT], &TaskAssignment::send_target_request, this, &MainWindow::send_target);

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
        int port = ui->lineEdit_port->text().toInt();
        ucli.connect_server(ip.c_str(), port);
    }

    if(!ucli.isTcpListen)
        ui->label->setText("Error on UDP connecting***");
    else
    {
//        qDebug() << "else...";
        if (cli.is_connected())
        {
            cli.disconnect_server();
            cli = TcpClient();
            ui->label->setText("Disconnected!!!");
            ui->button_connect->setText("connect");
            qDebug() << "disconnected";
        }
        else
        {
//            qDebug() << "else.. else..";
            ui->label->setText("Connecting...");
            std::string ip = ui->lineEdit_ip->text().toStdString();
            int port = ucli.tcp_port;
            cli.connect_server(ip.c_str(), port);
            qDebug() << "after connect...";
            if (cli.is_connected())
            {
                ui->label->setText("Connected!!!");
                ui->button_connect->setText("disconnect");
            }
            else
                ui->label->setText("Error on connecting***");
        }
    }
    if (cli.is_connected())
    {
        download_table();
    }
}

void MainWindow::download_table()
{
    if (cli.is_connected())
    {
        RobotSettings* rbt = (RobotSettings*)widgets[RBTSETTINGS_WGT];
        cli.download_table(rbt->rbt_table);
        rbt->set_table(rbt->rbt_table);
        Visualize* vlz = (Visualize*)widgets[VISUALIZE_WGT];
        vlz->update_joints_widget(rbt->rbt_table);
    }
}

void MainWindow::upload_table()
{
    if (cli.is_connected())
    {
        RobotSettings* rbt = (RobotSettings*)widgets[RBTSETTINGS_WGT];
        rbt->get_table(rbt->rbt_table);
        cli.upload_table(rbt->rbt_table);
        Visualize* vlz = (Visualize*)widgets[VISUALIZE_WGT];
        vlz->update_joints_widget(rbt->rbt_table);
    }
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
    if (cli.is_connected())
    {
        Eigen::VectorXf v;
        cli.get_current_pose(v);
        Visualize* vlz = (Visualize*)widgets[VISUALIZE_WGT];
        vlz->update_pose(v);
    }
}

void MainWindow::update_joints()
{
    if (cli.is_connected())
    {
        Eigen::VectorXf v;
        cli.get_current_joints(v);
        Visualize* vlz = (Visualize*)widgets[VISUALIZE_WGT];
        vlz->update_joints(v);
    }
}

void MainWindow::send_target(float target)
{
    if (cli.is_connected())
    {
        cli.send_target(target);
    }
}

void MainWindow::on_button_test_clicked()
{
    cli.test();
}


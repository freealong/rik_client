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
    bar = new SideBar(this);

    // set up background color;
    QPalette pal;
    pal.setColor(QPalette::Background, Qt::white);
    widgets[CONNECT_WGT]->setAutoFillBackground(true);
    widgets[CONNECT_WGT]->setPalette(pal);

    widgets[RBTSETTINGS_WGT]->setAutoFillBackground(true);
//    pal.setColor(QPalette::Background, Qt::yellow);
    widgets[RBTSETTINGS_WGT]->setPalette(pal);

    bar->setAutoFillBackground(true);
//    pal.setColor(QPalette::Background, Qt::black);
    bar->setPalette(pal);

    // move widgets to initial place;
    currentWidget = widgets[CONNECT_WGT];

    widgets[RBTSETTINGS_WGT]->move(0, - widgets[RBTSETTINGS_WGT]->height());
    bar->move(margin_x - bar->width(), 0);
    bar->setStatus(false);

    //
    connect(bar, &SideBar::connect_request, this, &MainWindow::show_connect);
    connect(bar, &SideBar::sidebar_request, this, &MainWindow::show_sidebar);
    connect(bar, &SideBar::rbtsettings_request, this, &MainWindow::show_rbtsettings);

    connect((RobotSettings*)widgets[RBTSETTINGS_WGT], &RobotSettings::table_request, this, &MainWindow::download_table);

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

//void MainWindow::on_pushButton_clicked()
//{
//    cli.connect_server("10.0.0.110", 8887);
//    Pose cur_pos;
//    cli.get_current_pose(cur_pos);
//    std::cout << cur_pos;
//}



void MainWindow::on_button_connect_clicked()
{
    if (cli.is_connected())
    {
        cli.disconnect_server();
        ui->label->setText("Disconnected!!!");
        ui->button_connect->setText("connect");
    }
    else
    {
        ui->label->setText("Connecting...");
        std::string ip = ui->lineEdit_ip->text().toStdString();
        int port = ui->lineEdit_port->text().toInt();
        cli.connect_server(ip.c_str(), port);
        if (cli.is_connected())
        {
            ui->label->setText("Connected!!!");
            ui->button_connect->setText("disconnect");
        }
        else
            ui->label->setText("Error on connecting***");
    }
}

void MainWindow::download_table()
{
    if (cli.is_connected())
    {
        dh_table t;
        cli.download_table(t);
        qDebug() << t[0].a << t[1].d;
    }
}

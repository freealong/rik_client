#include "sidebar.h"
#include "ui_sidebar.h"

SideBar::SideBar(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::SideBar),
    status(false)
{
    ui->setupUi(this);
}

SideBar::~SideBar()
{
    delete ui;
}

void SideBar::on_sidebar_button_clicked()
{
    emit sidebar_request(status);
}

void SideBar::on_connect_button_clicked()
{
    emit connect_request();
}

void SideBar::on_rbtsettings_botton_clicked()
{
    emit rbtsettings_request();
}

void SideBar::on_visualize_botton_clicked()
{
    emit visualize_request();
}

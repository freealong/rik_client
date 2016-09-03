#include "robotsettings.h"
#include "ui_robotsettings.h"

RobotSettings::RobotSettings(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::RobotSettings)
{
    ui->setupUi(this);
}

RobotSettings::~RobotSettings()
{
    delete ui;
}

void RobotSettings::on_button_download_clicked()
{
    emit table_request();
}

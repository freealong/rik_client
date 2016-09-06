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

void RobotSettings::set_table(dh_table &t)
{
    if (t.size() > 0)
        ui->tableWidget->setRowCount(t.size());
    for (unsigned i = 0; i < t.size(); ++i)
    {
        QTableWidgetItem *name = new QTableWidgetItem;
        QTableWidgetItem *alpha = new QTableWidgetItem;
        QTableWidgetItem *a = new QTableWidgetItem;
        QTableWidgetItem *d = new QTableWidgetItem;
        QTableWidgetItem *theta = new QTableWidgetItem;
        name->setText(QString::fromStdString(t[i].joint_name));
        alpha->setText(QString::number(t[i].alpha));
        a->setText(QString::number(t[i].a));
        d->setText(QString::number(t[i].d));
        theta->setText((QString::number(t[i].theta)));
        ui->tableWidget->setItem(i, 0, name);
        ui->tableWidget->setItem(i, 1, alpha);
        ui->tableWidget->setItem(i, 2, a);
        ui->tableWidget->setItem(i, 3, d);
        ui->tableWidget->setItem(i, 4, theta);
    }
}

void RobotSettings::get_table(dh_table &t)
{
    t.clear();
    dh_parametrs p;
    for (int i = 0; i < ui->tableWidget->rowCount(); i++)
    {
        p.joint_name = ui->tableWidget->item(i, 0)->text().toStdString();
        p.alpha = ui->tableWidget->item(i, 1)->text().toFloat();
        p.a = ui->tableWidget->item(i, 2)->text().toFloat();
        p.d = ui->tableWidget->item(i, 3)->text().toFloat();
        p.theta = ui->tableWidget->item(i, 4)->text().toFloat();
        t.push_back(p);
    }
}

void RobotSettings::on_button_download_clicked()
{
    emit download_request();
}

void RobotSettings::on_button_upload_clicked()
{
    emit upload_request();
}

void RobotSettings::on_button_add_clicked()
{
    ui->tableWidget->setRowCount(ui->tableWidget->rowCount() + 1);
}

void RobotSettings::on_button_del_clicked()
{
    int current_row = ui->tableWidget->currentRow();
    if (current_row != -1)
        ui->tableWidget->removeRow(current_row);
}

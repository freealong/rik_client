#include "taskassignment.h"
#include "ui_taskassignment.h"

#include <QDebug>

TaskAssignment::TaskAssignment(QWidget *parent) :
    MyWidget(parent),
    ui(new Ui::TaskAssignment)
{
    ui->setupUi(this);
}

TaskAssignment::~TaskAssignment()
{
    delete ui;
}

void TaskAssignment::on_pushButton_clicked()
{

    Eigen::VectorXf target(6);
    QStringList target_list = ui->lineEdit->text().split(",");
    if (target_list.size() < 6)
    {
        qDebug()<<"target has 6 element and seperate by ,";
        return;
    }
    for (int i = 0; i < 6; ++i)
    {
        target(i) = target_list.at(i).toFloat();
    }
    emit send_target_request(target);
}

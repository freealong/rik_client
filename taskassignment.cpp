#include "taskassignment.h"
#include "ui_taskassignment.h"

#include <QDebug>

TaskAssignment::TaskAssignment(QWidget *parent) :
    MyWidget(parent),
    ui(new Ui::TaskAssignment),
    show_joints(false)
{
    ui->setupUi(this);
    ui->tab_pose->setDisabled(true);
}

TaskAssignment::~TaskAssignment()
{
    delete ui;
}

void TaskAssignment::update_widget(int n)
{
    int size = ui->listWidget->count();
    if (size == n)
        return;
    else if (size > n)
    {
        for (int i = size-1; i > n-1; --i)
            ui->listWidget->takeItem(i);
    }
    else
    {
        for (int i = size; i < n; ++i)
        {
            ui->listWidget->addItem("0");
            ui->listWidget->openPersistentEditor(ui->listWidget->item(i));
        }
    }

//    ui->listWidget_actual_position->clear();
//    for(int i=0;i<size;i++)
//    {
//        int data = robot
//        ui->listWidget_actual_position->a
//    }
}

void TaskAssignment::mode_0_init()
{
//    emit joints_request(true);
}

void TaskAssignment::on_set_mode_btn_clicked()
{
    int mode = ui->comboBox->currentIndex();
    ui->tabWidget->setCurrentIndex(mode);
    switch (mode)
    {
    case 0 :
        ui->tab_joints->setEnabled(true);
        ui->tab_pose->setEnabled(false);
        mode_0_init();
        break;
    case 1 :
        ui->tab_joints->setEnabled(false);
        ui->tab_pose->setEnabled(true);
        break;
    default:
        ui->tab_joints->setEnabled(false);
        ui->tab_pose->setEnabled(false);
    }
    emit send_mode_request(mode);
}

void TaskAssignment::on_send_joints_btn_clicked()
{
    int size = ui->listWidget->count();
    Eigen::VectorXf target(size);
    for (int i = 0; i < size; ++i) {
        target(i) = ui->listWidget->item(i)->text().toFloat();
    }
    emit send_target_request(target);
}

void TaskAssignment::on_send_pose_btn_clicked()
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


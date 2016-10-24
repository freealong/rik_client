#include "taskassignment.h"
#include "ui_taskassignment.h"

#include <QDebug>

TaskAssignment::TaskAssignment(QWidget *parent) :
    MyWidget(parent),
    ui(new Ui::TaskAssignment),
    show_joints(false),
    show_pose(false)
{
    ui->setupUi(this);
    ui->tab_joints->setDisabled(true);
    ui->tab_pose->setDisabled(true);
//    ui->tab_path->setDisabled(true);
}

TaskAssignment::~TaskAssignment()
{
    delete ui;
}

void TaskAssignment::reset()
{
    ui->lineEdit->clear();
    ui->listWidget->clear();
    ui->actual_joint_LineEdit->clear();
    ui->actual_pose_LineEdit->clear();
    ui->target_path_LineEdit->clear();
    ui->tab_joints->setDisabled(true);
    ui->tab_pose->setDisabled(true);
    ui->tab_path->setDisabled(true);
}

void TaskAssignment::update_widget(int n)
{
    int size = ui->listWidget->count();
    if (size == n)
        return;
    else if (size > n)
    {
        for (int i = size-1; i > n-1; --i)
        {
            ui->listWidget->takeItem(i);
        }
    }
    else
    {
        for (int i = size; i < n; ++i)
        {
            ui->listWidget->addItem("0");
            ui->listWidget->openPersistentEditor(ui->listWidget->item(i));
        }
    }
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
        ui->tab_path->setEnabled(false);
        break;
    case 1 :
        ui->tab_joints->setEnabled(false);
        ui->tab_pose->setEnabled(true);
        ui->tab_path->setEnabled(false);
        break;
    case 2:
        ui->tab_joints->setEnabled(false);
        ui->tab_pose->setEnabled(false);
        ui->tab_path->setEnabled(true);
        break;
    default:
        ui->tab_joints->setEnabled(false);
        ui->tab_pose->setEnabled(false);
        ui->tab_path->setEnabled(false);
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
    emit send_target_request(target, 0);
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
    emit send_target_request(target, 1);
}

void TaskAssignment::on_send_path_btn_clicked()
{
    Eigen::VectorXf target(7);
    QStringList target_list = ui->target_path_LineEdit->text().split(",");
    if (target_list.size() < 7)
    {
        qDebug()<<"target has 7 element and seperate by ,";
        return;
    }
    for (int i = 0; i < 7; ++i)
    {
        target(i) = target_list.at(i).toFloat();
    }
    emit send_target_request(target, 2);
}

void TaskAssignment::on_test_btn_clicked()
{
    reset();
}

void TaskAssignment::on_show_joints_btn_clicked()
{
    show_joints = !show_joints;
    emit send_joints_request(show_joints);
}

void TaskAssignment::update_joints(const Eigen::VectorXf &v)
{
    if (!show_joints)
        return;
    QString s;
    for (int i = 0; i < v.rows(); ++i)
    {
        s += QString::number(v(i));
        if (i < v.rows() - 1)
            s += ",";
    }
    ui->actual_joint_LineEdit->setText(s);
}

void TaskAssignment::update_pose(const Eigen::VectorXf &v)
{
    if (!show_pose)
        return;
    QString s;
    for (int i = 0; i < v.rows(); ++i)
    {
        s += QString::number(v(i));
        if (i < v.rows() - 1)
            s += ",";
    }
    ui->actual_pose_LineEdit->setText(s);
}

void TaskAssignment::on_show_pose_btn_clicked()
{
    show_pose = !show_pose;
    emit send_pose_request(show_pose);
}


#include "taskassignment.h"
#include "ui_taskassignment.h"

#include <QDebug>

TaskAssignment::TaskAssignment(QWidget *parent) :
    MyWidget(parent),
    ui(new Ui::TaskAssignment),
    show_joints(false),
    show_pose(false),
    mode(-1),
    pressed_btn_id(-1)
{
    ui->setupUi(this);
    ui->tab_joints->setDisabled(true);
    ui->tab_pose->setDisabled(true);
    ui->tab_path->setDisabled(true);

    // set up pose mode widget
    for (int i = 0; i < 6; i++)
    {
        QPushButton* btn_minus = new QPushButton("-");
        QPushButton* btn_plus = new QPushButton("+");
        connect(btn_minus, &QPushButton::pressed, this, &TaskAssignment::on_increasing_btn_pressed);
        connect(btn_minus, &QPushButton::released, this, &TaskAssignment::on_increasing_btn_released);
        connect(btn_plus, &QPushButton::pressed, this, &TaskAssignment::on_increasing_btn_pressed);
        connect(btn_plus, &QPushButton::released, this, &TaskAssignment::on_increasing_btn_released);
        ui->pose_minus_vl->addWidget(btn_minus);
        ui->pose_plus_vl->addWidget(btn_plus);
    }

    // set up path mode widget
    for (int i = 0; i < 6; i++)
    {
        QLineEdit* target_le = new QLineEdit();
        ui->target_pose_vl->addWidget(target_le);
        if (ui->path_type_cm->currentText() == QString("arc path"))
        {
            QLineEdit* middle_le = new QLineEdit();
            ui->middle_pose_vl->addWidget(middle_le);
        }
    }

    QLabel* name = new QLabel("Time:");
    ui->speed_vl->addWidget(name);
    QLineEdit* speed_le = new QLineEdit();
    ui->speed_vl->addWidget(speed_le);
    if (ui->path_type_cm->currentText() == QString("arc path"))
    {
        name->setText("speed:");
        QLabel* acc_label = new QLabel("acc:");
        ui->speed_vl->addWidget(acc_label);
        QLineEdit* acc_le = new QLineEdit();
        ui->speed_vl->addWidget(acc_le);
    }

    connect(ui->path_type_cm, SIGNAL(currentIndexChanged(int)), this, SLOT(on_path_type_changed()));
}

TaskAssignment::~TaskAssignment()
{
    delete ui;
}

void TaskAssignment::reset()
{
//    ui->lineEdit_x->clear();
//    ui->lineEdit_y->clear();
//    ui->lineEdit_z->clear();
//    ui->lineEdit_a->clear();
//    ui->lineEdit_b->clear();
//    ui->lineEdit_c->clear();
//    ui->listWidget->clear();
//    ui->actual_joint_LineEdit->clear();
//    ui->actual_pose_LineEdit->clear();
//    ui->target_path_LineEdit->clear();
    ui->tab_joints->setDisabled(true);
    ui->tab_pose->setDisabled(true);
    ui->tab_path->setDisabled(true);
}

void TaskAssignment::update_widget(int n)
{
    // set up joints mode widget
    int s = ui->joints_name_vl->count();
    if (s > n)
    {
        QLayoutItem *item;
        for (int i = s-1; i > n-1; --i)
        {
            item = ui->joints_name_vl->takeAt(i);
            delete item->widget();
            item = ui->joints_value_vl->takeAt(i);
            delete item->widget();
            item = ui->joints_minus_vl->takeAt(i);
            delete item->widget();
            item = ui->joints_plus_vl->takeAt(i);
            delete item->widget();
        }
    }
    else if (s < n)
    {
        for (int i = s; i < n; ++i)
        {
            QLabel* name = new QLabel("joint "+QString::number(i));
            ui->joints_name_vl->addWidget(name);
            QLineEdit* value = new QLineEdit();
            ui->joints_value_vl->addWidget(value);
            QPushButton* btn_minus = new QPushButton("-");
            QPushButton* btn_plus = new QPushButton("+");
            connect(btn_minus, &QPushButton::pressed, this, &TaskAssignment::on_increasing_btn_pressed);
            connect(btn_minus, &QPushButton::released, this, &TaskAssignment::on_increasing_btn_released);
            connect(btn_plus, &QPushButton::pressed, this, &TaskAssignment::on_increasing_btn_pressed);
            connect(btn_plus, &QPushButton::released, this, &TaskAssignment::on_increasing_btn_released);
            ui->joints_minus_vl->addWidget(btn_minus);
            ui->joints_plus_vl->addWidget(btn_plus);
        }
    }
}

void TaskAssignment::on_set_mode_btn_clicked()
{
    mode = ui->comboBox->currentIndex();
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
    int size = ui->joints_name_vl->count();
    Eigen::VectorXf target(size);
    for (int i = 0; i < size; ++i) {
        target(i) = ((QLineEdit*)(ui->joints_value_vl->itemAt(i)->widget()))->text().toFloat();
    }
    emit send_target_request(target, 0);
}

void TaskAssignment::on_send_pose_btn_clicked()
{
    Eigen::VectorXf target(6);
    target(0) = ui->lineEdit_x->text().toFloat();
    target(1) = ui->lineEdit_y->text().toFloat();
    target(2) = ui->lineEdit_z->text().toFloat();
    target(3) = ui->lineEdit_a->text().toFloat();
    target(4) = ui->lineEdit_b->text().toFloat();
    target(5) = ui->lineEdit_c->text().toFloat();

    emit send_target_request(target, 1);
}

void TaskAssignment::on_send_path_btn_clicked()
{
    int target_sz = 0;
    if (ui->path_type_cm->currentText() == QString("line path"))
        target_sz = 7;
    else if (ui->path_type_cm->currentText() == QString("arc path"))
        target_sz = 14;
    Eigen::VectorXf target(target_sz);

    if (target_sz == 7)
    {
        for (int i = 0; i < 6; ++i)
            target(i) = ((QLineEdit*)(ui->target_pose_vl->itemAt(i)->widget()))->text().toFloat();
        target(6) = ((QLineEdit*)(ui->speed_vl->itemAt(1)->widget()))->text().toFloat();
    }
    else if (target_sz == 14)
    {
        for (int i = 0; i < 6; ++i)
        {
            target(i) = ((QLineEdit*)(ui->target_pose_vl->itemAt(i)->widget()))->text().toFloat();
            target(i+6) = ((QLineEdit*)(ui->middle_pose_vl->itemAt(i)->widget()))->text().toFloat();
        }
        target(12) = ((QLineEdit*)(ui->speed_vl->itemAt(1)->widget()))->text().toFloat();
        target(13) = ((QLineEdit*)(ui->speed_vl->itemAt(3)->widget()))->text().toFloat();
    }
    emit send_target_request(target, 2);
}

void TaskAssignment::on_test_btn_clicked()
{
    update_widget(6);
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

void TaskAssignment::on_increasing_btn_pressed()
{
    bool is_minus_down;
    bool is_plus_down;
    for (int i = 0; i < ui->joints_minus_vl->count(); ++i)
    {
        is_minus_down = ((QPushButton*)(ui->joints_minus_vl->itemAt(i)->widget()))->isDown() ||
                 ((QPushButton*)(ui->pose_minus_vl->itemAt(i)->widget()))->isDown();
        is_plus_down = ((QPushButton*)(ui->joints_plus_vl->itemAt(i)->widget()))->isDown() ||
                 ((QPushButton*)(ui->pose_plus_vl->itemAt(i)->widget()))->isDown();
        if (is_minus_down)
        {
            pressed_btn_id = i;
            qDebug() << "mode: " << mode << ", decreasing " << i;
            emit send_increasing_request(true, mode, i, -0.1);
            break;
        }
        if (is_plus_down)
        {
            pressed_btn_id = i;
            qDebug() << "mode: " << mode << ", increasing " << i;
            emit send_increasing_request(true, mode, i, 0.1);
            break;
        }
    }
}

void TaskAssignment::on_increasing_btn_released()
{
    if (pressed_btn_id != -1)
    {
        if (mode == 0)
            emit send_increasing_request(false, 0, pressed_btn_id, 0);
        else if (mode == 1)
            emit send_increasing_request(false, 1, pressed_btn_id, 0);
        qDebug() << "stop mode " << mode << ", id: " << pressed_btn_id;
        pressed_btn_id = -1;
    }
}

void TaskAssignment::on_path_type_changed()
{
    if (ui->path_type_cm->currentText() == QString("line path"))
    {
        QLayoutItem* child;
        while(ui->middle_pose_vl->count()!=0)
        {
            child = ui->middle_pose_vl->takeAt(0);
            delete child->widget();
            delete child;
        }
        child = ui->speed_vl->takeAt(3);
        delete child->widget();
        delete child;
        child = ui->speed_vl->takeAt(2);
        delete child->widget();
        delete child;
        ((QLabel*)(ui->speed_vl->itemAt(0)->widget()))->setText("time:");
    }
    else if (ui->path_type_cm->currentText() == QString("arc path"))
    {
        for (int i = 0; i < 6; i++)
        {
            QLineEdit* middle_le = new QLineEdit();
            ui->middle_pose_vl->addWidget(middle_le);
        }
        QLabel* acc_label = new QLabel("acc:");
        ui->speed_vl->addWidget(acc_label);
        QLineEdit* acc_le = new QLineEdit();
        ui->speed_vl->addWidget(acc_le);
        ((QLabel*)(ui->speed_vl->itemAt(0)->widget()))->setText("speed:");
    }
}

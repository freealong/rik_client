#include "taskassignment.h"
#include "ui_taskassignment.h"

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
    emit send_target_request(ui->lineEdit->text().toFloat());
}

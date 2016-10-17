#ifndef TASKASSIGNMENT_H
#define TASKASSIGNMENT_H

#include <mywidget.h>

#include <Eigen>

namespace Ui {
class TaskAssignment;
}

class TaskAssignment : public MyWidget
{
    Q_OBJECT

public:
    explicit TaskAssignment(QWidget *parent = 0);
    ~TaskAssignment();

signals:
    void send_target_request(Eigen::VectorXf);

private slots:
    void on_pushButton_clicked();

private:
    Ui::TaskAssignment *ui;
};

#endif // TASKASSIGNMENT_H

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

    void update_widget(int n);
    void mode_0_init();
signals:
    void send_target_request(Eigen::VectorXf);
    void send_mode_request(int);
//    void joints_request(bool);

private slots:
    void on_set_mode_btn_clicked();

    void on_send_joints_btn_clicked();

    void on_send_pose_btn_clicked();

private:
    Ui::TaskAssignment *ui;
    bool show_joints;
};

#endif // TASKASSIGNMENT_H

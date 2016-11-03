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

    void reset();
    void update_widget(int n);
    void update_joints(const Eigen::VectorXf& v);
    void update_pose(const Eigen::VectorXf& v);

signals:
    void send_target_request(Eigen::VectorXf, int);
    void send_mode_request(int);
    void send_joints_request(bool);
    void send_pose_request(bool);
    void send_increasing_request(bool, int, int, float);

private slots:
    void on_set_mode_btn_clicked();

    void on_send_joints_btn_clicked();

    void on_send_pose_btn_clicked();

    void on_send_path_btn_clicked();

    void on_test_btn_clicked();

    void on_show_joints_btn_clicked();

    void on_show_pose_btn_clicked();

    void on_increasing_btn_pressed();
    void on_increasing_btn_released();

    void on_path_type_changed();


private:
    Ui::TaskAssignment *ui;
    bool show_joints;
    bool show_pose;
    int mode;
    int pressed_btn_id;
};

#endif // TASKASSIGNMENT_H

#ifndef VISUALIZE_H
#define VISUALIZE_H

#include <QWidget>
#include <Eigen>

#include "qcustomplot.h"
#include "sharetypes.h"

namespace Ui {
class Visualize;
}

class Visualize : public QWidget
{
    Q_OBJECT

public:
    explicit Visualize(QWidget *parent = 0);
    ~Visualize();

    void update_pose(Eigen::VectorXf &v);
    void update_joints(Eigen::VectorXf &v);
    void update_joints_widget(dh_table& t);

signals:
    void pose_request(bool);
    void joints_request(bool);

private slots:
    void on_button_pose_clicked();

    void on_button_joints_clicked();

private:
    Ui::Visualize *ui;
    QCustomPlot* plot_pose;
    QCustomPlot* plot_joints;
    bool show_pose;
    bool show_joints;

};

#endif // VISUALIZE_H

#include "visualize.h"
#include "ui_visualize.h"

Visualize::Visualize(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Visualize),
    show_pose(false),
    show_joints(false)
{
    ui->setupUi(this);
}

Visualize::~Visualize()
{
    delete ui;
}

void Visualize::update_pose(Eigen::VectorXf &v)
{
    // @TODO plot pose
    QString pose("pose: ");
    for (unsigned i = 0; i < v.size(); i++)
    {
        pose += QString::number(v(i));
        pose += QString(", ");
    }
    ui->label_pose->setText(pose);
}

void Visualize::update_joints(Eigen::VectorXf &v)
{
    // @TODO plot joints
    QString joints("joints: ");
    for (unsigned i = 0; i < v.size(); i++)
    {
        joints += QString::number(v(i));
        joints += QString(", ");
    }
    ui->label_joints->setText(joints);
}

void Visualize::on_button_pose_clicked()
{
    show_pose = !show_pose;
    emit pose_request(show_pose);
}

void Visualize::on_button_joints_clicked()
{
    show_joints = !show_joints;
    emit joints_request(show_joints);
}

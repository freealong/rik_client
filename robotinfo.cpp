#include "robotinfo.h"

RobotInfo::RobotInfo():
    joints_num(-1)
{
}

void RobotInfo::update_table(const dh_table &t)
{
    table = t;
    joints_num = t.size();
}

void RobotInfo::update_joints_limits(const joints_limits &jl)
{
    limit = jl;
}

void RobotInfo::update_joints(const Eigen::VectorXf &j)
{
    joints = j;
}

void RobotInfo::update_pose(const Eigen::VectorXf &p)
{
    pose = p;
}


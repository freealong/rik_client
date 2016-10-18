#ifndef ROBOTINFO_H
#define ROBOTINFO_H

#include "sharetypes.h"
#include <Eigen>

class RobotInfo
{
public:
    RobotInfo();
    int joints_num;
    dh_table table;
    Eigen::VectorXf joints;
    Eigen::VectorXf pose;
};

#endif // ROBOTINFO_H

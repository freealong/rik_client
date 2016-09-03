#ifndef POSE_H
#define POSE_H

#include <iostream>
#include <Eigen>

class Pose
{
public:
    Eigen::Vector3f position;
    Eigen::Vector3f posture;

    friend std::ostream& operator<<(std::ostream& os, const Pose& p);
};


#endif // POSE_H

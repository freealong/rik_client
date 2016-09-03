#include "pose.h"

std::ostream& operator<<(std::ostream& os, const Pose& p)
{
    os << p.position << std::endl;
    os << p.posture;
    return os;
}

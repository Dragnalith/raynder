#ifndef __RAYNDER_COORDINATE_SYSTEM_HPP__
#define __RAYNDER_COORDINATE_SYSTEM_HPP__

#include "glm/glm.hpp"

struct CoordinateSystem
{
	glm::vec3 Origin;
    glm::vec3 X;
    glm::vec3 Y;
    glm::vec3 Z;
};

#endif

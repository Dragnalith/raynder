#ifndef __RAYNDER_MATERIAL_HPP__
#define __RAYNDER_MATERIAL_HPP__

#include "glm/glm.hpp"

struct Material
{
    Material(glm::vec3 const& albedo) : Albedo(albedo) {}

    glm::vec3 Albedo;
};

#endif

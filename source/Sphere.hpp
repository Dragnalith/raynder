#ifndef __RAYNDER_SPHERE_HPP__
#define __RAYNDER_SPHERE_HPP__

#include "glm/glm.hpp"

#include "Object.hpp"
#include "Material.hpp"

class Sphere : public Object
{
public:
    Sphere(glm::vec3 const& center, float const radius);

    bool Intersect(Ray const& ray, Intersection* pIntersection) const override;

private:
    glm::vec3 m_Center;
    float     m_Radius;
    Material  m_Material;
};

#endif

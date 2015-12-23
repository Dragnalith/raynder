#ifndef __RAYNDER_OBJECT_HPP__
#define __RAYNDER_OBJECT_HPP__

#include "Material.hpp"

class Intersection;
class Ray;

class Object
{
public:
    Object(Material const& material);

    inline Material const* GetMaterial() const { return &m_Material; };
    virtual bool Intersect(Ray const& ray, Intersection* pIntersection) const = 0;
    virtual float Surface() const = 0;
    virtual glm::vec3 SampleUniform() const = 0;

protected:
    Material m_Material;
};

#endif

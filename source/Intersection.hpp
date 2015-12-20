#ifndef __RAYNDER_INTERSECTION_HPP__
#define __RAYNDER_INTERSECTION_HPP__

#include "glm/glm.hpp"

class Material;

class Intersection
{
public:
    Intersection();
    Intersection(float const distance, glm::vec3 const& normal, Material const* pMaterial);
    Material const* GetMaterial() const;
    float           GetDistance() const;
    glm::vec3 const& GetNormal() const;
    bool            HasIntersection() const;

private:
    float            m_Distance;
    glm::vec3        m_Normal;
    Material const*  m_pMaterial;
};

#endif

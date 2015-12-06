#ifndef __RAYNDER_INTERSECTION_HPP__
#define __RAYNDER_INTERSECTION_HPP__

#include "glm/glm.hpp"

struct Material;

class Intersection
{
public:
    Intersection() = default;
    Intersection(glm::vec3 const& position, glm::vec3 const& normal, Material const* pMaterial);
    Material const* GetMaterial() const;

private:
    glm::vec3        m_Position;
    glm::vec3        m_Normal;
    Material const*  m_pMaterial;
};

#endif

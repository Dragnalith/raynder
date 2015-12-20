#include "Intersection.hpp"
#include "Material.hpp"

Intersection::Intersection()
    : m_Distance(std::numeric_limits<float>::max())
{
}

Intersection::Intersection(float distance, glm::vec3 const& normal, Material const* pMaterial)
    : m_Distance(distance)
    , m_pMaterial(pMaterial)
{
    m_Normal = glm::normalize(normal);
}

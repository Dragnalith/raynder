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

Material const* Intersection::GetMaterial() const
{
    return m_pMaterial;
}

float Intersection::GetDistance() const
{
    return m_Distance;
}

glm::vec3 const& Intersection::GetNormal() const
{
    return m_Normal;
}


bool Intersection::HasIntersection() const
{
    return m_Distance != std::numeric_limits<float>::max();
}

#include "Intersection.hpp"
#include "Material.hpp"

Intersection::Intersection()
    : m_Distance(std::numeric_limits<float>::max())
{
}

Intersection::Intersection(float distance, glm::vec3 const& normal, Material const* pMaterial)
    : m_Distance(distance)
    , m_Normal(normal)
    , m_pMaterial(pMaterial)
{
}

Material const* Intersection::GetMaterial() const
{
    return m_pMaterial;
}

float Intersection::GetDistance() const
{
    return m_Distance;
}

bool Intersection::HasIntersection() const
{
    return m_Distance != std::numeric_limits<float>::max();
}

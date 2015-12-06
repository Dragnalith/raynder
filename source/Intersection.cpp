#include "Intersection.hpp"
#include "Material.hpp"

Intersection::Intersection(glm::vec3 const& position, glm::vec3 const& normal, Material const* pMaterial)
    : m_Position(position)
    , m_Normal(normal)
    , m_pMaterial(pMaterial)
{
}

Material const* Intersection::GetMaterial() const
{
    return m_pMaterial;
}

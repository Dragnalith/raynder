#include "Ray.hpp"

#include "glm/glm.hpp"

Ray::Ray(glm::vec3 const& origin, glm::vec3 const& direction)
    : m_Origin(origin)
    , m_Direction(direction)
{
    m_Direction = glm::normalize(m_Direction);
}


glm::vec3 const& Ray::GetDirection() const
{
    return m_Direction;
}

glm::vec3 const& Ray::GetOrigin() const
{
    return m_Origin;
}

glm::vec3 Ray::GetPoint(float const distance) const
{
    return m_Origin + distance * m_Direction;
}

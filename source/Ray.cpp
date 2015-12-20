#include "Ray.hpp"

#include "glm/glm.hpp"

Ray::Ray(glm::vec3 const& origin, glm::vec3 const& direction)
    : m_Origin(origin)
{
    m_Direction = glm::normalize(direction);
}

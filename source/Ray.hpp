#ifndef __RAYNDER_RAY_HPP__
#define __RAYNDER_RAY_HPP__

#include "glm/glm.hpp"

class Ray
{
public:
    Ray(glm::vec3 const& origin, glm::vec3 const& direction);
    
    inline glm::vec3 const& GetDirection() const { return m_Direction; }
    inline glm::vec3 const& GetOrigin() const { return m_Origin; }
    inline glm::vec3 GetPoint(float distance) const { return m_Origin + distance * m_Direction; };

private:
    glm::vec3 m_Origin;
    glm::vec3 m_Direction;
};

#endif

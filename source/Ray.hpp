#ifndef __RAYNDER_RAY_HPP__
#define __RAYNDER_RAY_HPP__

#include "glm/glm.hpp"

class Ray
{
public:
    Ray(glm::vec3 const& origin, glm::vec3 const& direction);
    
    glm::vec3 const& GetDirection() const;
    glm::vec3 const& GetOrigin() const;

private:
    glm::vec3 m_Origin;
    glm::vec3 m_Direction;
};

#endif

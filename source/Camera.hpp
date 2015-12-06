#ifndef __RAYNDER_CAMERA_HPP__
#define __RAYNDER_CAMERA_HPP__

#include "glm/glm.hpp"

struct CoordinateSystem;

class Camera
{
public:
    Camera(glm::vec3 const& position, glm::vec3 const& front, glm::vec3 const& up,
        float bottom, float top, float left, float right);

    CoordinateSystem GetScreenSpaceCoordinateSystem() const;
    glm::vec3 GetPosition() const;

private:
    glm::vec3 m_Position;
    glm::vec3 m_Front;
    glm::vec3 m_Up;

    float     m_Bottom;
    float     m_Top;
    float     m_Left;
    float     m_Right;
};

#endif

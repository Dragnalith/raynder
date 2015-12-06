#include "Camera.hpp"
#include "CoordinateSystem.hpp"

#include "glm/glm.hpp"

Camera::Camera(glm::vec3 const& position, glm::vec3 const& front, glm::vec3 const& up,
    float bottom, float top, float left, float right)
    : m_Position(position)
    , m_Front(front)
    , m_Up(up)
    , m_Bottom(bottom)
    , m_Top(top)
    , m_Left(left)
    , m_Right(right)
{
}

CoordinateSystem Camera::GetScreenSpaceCoordinateSystem() const
{
    CoordinateSystem system;
    auto right = glm::cross(m_Front, m_Up);
    right = glm::normalize(right);


    system.Origin = m_Position + m_Front + m_Top * m_Up + m_Left * right;
    system.X = (m_Right - m_Left) * right;
    system.Y = (m_Bottom - m_Top) * m_Up;
    system.Z = -glm::normalize(m_Front);

    return system;
}

glm::vec3 Camera::GetPosition() const
{
    return m_Position;
}

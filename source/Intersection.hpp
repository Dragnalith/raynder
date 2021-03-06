#ifndef __RAYNDER_INTERSECTION_HPP__
#define __RAYNDER_INTERSECTION_HPP__

#include "glm/glm.hpp"

#include "Object.hpp"

class Intersection
{
public:
    Intersection();
    Intersection(float const distance, glm::vec3 const& normal, Object const* pObject);
    inline Object const* GetObject() const { return m_pObject; }
    inline Material const* GetMaterial() const { return m_pObject->GetMaterial(); }
    inline float const&    GetDistance() const { return m_Distance; }
    inline glm::vec3 const& GetNormal() const { return m_Normal; }
    bool            HasIntersection() const { return m_Distance != std::numeric_limits<float>::max(); }

private:
    float            m_Distance;
    glm::vec3        m_Normal;
    Object const*    m_pObject;
};

#endif

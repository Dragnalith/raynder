#include "Sphere.hpp"
#include "Ray.hpp"
#include "Intersection.hpp"
#include "Helper.hpp"

#include "glm/glm.hpp"

Sphere::Sphere(glm::vec3 const& center, float const radius, Material const& material)
    : m_Center(center)
    , m_Radius(radius)
    , m_Material(material)
{
}

namespace
{

bool sphere_intersection(Ray const&ray, glm::vec3 const&O, float const R, float* pDistance)
{
    // Note(mdelorme): To understand variable notation:
    // I consider a sphere from center O with a radius R, and a ray
    // with an origin P and a direction u.

    glm::vec3 const& u  = ray.GetDirection();
    glm::vec3 const& OP = ray.GetOrigin() - O;

    // Note(mdelorme): We wanna solve the equation
    // t² + 2*CP.u*t + CP²-R² = 0
    // t² + 2*B*t    + C      = 0
    
    float const B = glm::dot(OP, u);
    float const C = glm::dot(OP, OP) - R * R;
    float const delta = B * B - C;

    if (delta < 0.f)
    {
        return false;
    }

    float const delta_sqrt = sqrt(delta);
    
    *pDistance = -B - delta_sqrt;
    if (*pDistance < 0)
    {
        *pDistance = -B + delta_sqrt;
    }

    return true;
}

}

bool Sphere::Intersect(Ray const& ray, Intersection* pIntersection) const 
{
    DRGN_ASSERT(pIntersection != nullptr);

    float distance;
    if (!sphere_intersection(ray, m_Center, m_Radius, &distance))
    {
        return false;
    }

    glm::vec3 const position = ray.GetPoint(distance);
    glm::vec3 const normal   = position - m_Center;

    if (glm::dot(normal, ray.GetDirection()) < 0)
    {
        *pIntersection = Intersection(distance, normal, &m_Material);
    } 
    else
    {
        *pIntersection = Intersection(distance, -normal, &m_Material);
    }

    return true;
}

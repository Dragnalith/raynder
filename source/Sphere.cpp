#include "Sphere.hpp"
#include "Ray.hpp"
#include "Intersection.hpp"
#include "Helper.hpp"

#include "glm/glm.hpp"

#include "Sampler.hpp"

Sphere::Sphere(glm::vec3 const& center, float const radius, Material const& material)
    : Object(material)
    , m_Center(center)
    , m_Radius(radius)
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
    // t� + 2*CP.u*t + CP�-R� = 0
    // t� + 2*B*t    + C      = 0
    
    float const B = glm::dot(OP, u);
    float const C = glm::dot(OP, OP) - R * R;
    float const delta = B * B - C;

    if (delta < 0.f)
    {
        return false;
    }

    float const delta_sqrt = sqrt(delta);
    float constexpr epsilon = 0.0001f;
    *pDistance = -B - delta_sqrt;
    if (*pDistance < epsilon)
    {
        *pDistance = -B + delta_sqrt;
        if (*pDistance < epsilon)
        {
            return false;
        }
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
        *pIntersection = Intersection(distance, normal, this);
    } 
    else
    {
        *pIntersection = Intersection(distance, -normal, this);
    }

    return true;
}

float Sphere::Surface() const
{
    return 4 * drgn::Pi * m_Radius * m_Radius;
}

glm::vec3 Sphere::SampleUniform() const
{
    return m_Radius * Sampler::UniformSphericalDirection() + m_Center;
}

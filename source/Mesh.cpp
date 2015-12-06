#include "Mesh.hpp"

#include "Helper.hpp"
#include "Ray.hpp"
#include "Intersection.hpp"

Mesh::Mesh(glm::vec3 const* vertexBuffer, size_t size, Material const& material)
    : m_VertexBuffer(vertexBuffer, vertexBuffer + size)
    , m_Material(material)
{
    DRGN_ASSERT(m_VertexBuffer.size() % 3 == 0);
}

namespace
{
float const Epsilon = 0.00001f;

bool triangle_intersection(Ray const&ray, glm::vec3 const&p0, glm::vec3 const& p1, glm::vec3 const& p2, float* pDistance)
{
    DRGN_ASSERT_NOT_IMPLEMENTED();
    return false;

    // TODO(mdelorme): Implement Möller–Trumbore intersection algorithm
    /*
    glm::vec3 const e0 = p1 - p0;
    glm::vec3 const e1 = p2 - p0;

    glm::vec3 const d = ray.GetDirection();

    glm::vec3 const P = glm::cross(e1, d);
    float const det = glm::dot(e0, P);

    if (-Epsilon < det && det < Epsilon)
    {
        return false;
    }
    
    float const inv_det = 1.f / det;
    glm::vec3 const T = ray.GetOrigin() - p0;
    float const u = glm::dot(T, P) * inv_det;

    if (u < 0.f || 1.f < u)
    {
        return false;
    }

    glm::vec3 const Q = glm::cross(T, e0);
    float const v = glm::dot(d, Q) * inv_det;

    if (v < 0.f || 1.f < v)
    {
        return false;
    }

    *pDistance = -dot(e1, Q) * inv_det;

    if (*pDistance >= 0.f)
    {
        return true;
    }
    else
    {
        return false;
    }
    */
}

}

bool Mesh::Intersect(Ray const& ray, Intersection* pIntersection) const
{
    DRGN_ASSERT(pIntersection != nullptr);

    float nearest_distance = std::numeric_limits<float>::max();
    glm::vec3 normal;

    for (size_t i = 0; i < m_VertexBuffer.size(); i += 3)
    {
        glm::vec3 const& p0 = m_VertexBuffer[i];
        glm::vec3 const& p1 = m_VertexBuffer[i + 1];
        glm::vec3 const& p2 = m_VertexBuffer[i + 2];

        float distance;
        if (!triangle_intersection(ray, p0, p1, p2, &distance))
        {
            continue;
        }

        if (distance >= 0 && distance < nearest_distance)
        {
            nearest_distance = distance;
            normal = glm::cross(p1 - p0, p2 - p0);
        }
    }
    
    if (nearest_distance == std::numeric_limits<float>::max())
    {
        return false;
    }

    *pIntersection = Intersection(nearest_distance, normal, &m_Material);

    return true;
}

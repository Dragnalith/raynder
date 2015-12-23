#include "Mesh.hpp"

#include "Helper.hpp"
#include "Ray.hpp"
#include "Intersection.hpp"

Mesh::Mesh(glm::vec3 const* vertexBuffer, size_t size, Material const& material)
    : Object(material)
    , m_VertexBuffer(vertexBuffer, vertexBuffer + size)
{
    DRGN_ASSERT(m_VertexBuffer.size() % 3 == 0);
}

namespace
{
float const Epsilon = 0.00001f;

bool triangle_intersection(Ray const&ray, glm::vec3 const&p0, glm::vec3 const& p1, glm::vec3 const& p2, float* pDistance)
{
    glm::vec3 const e0 = p1 - p0;
    glm::vec3 const e1 = p2 - p0;

    glm::vec3 const& d = ray.GetDirection();

    glm::vec3 const P = glm::cross(d, e1);
    float const det = glm::dot(P, e0);

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

    if (v < 0.f || 1.f < (u + v))
    {
        return false;
    }

    *pDistance = dot(e1, Q) * inv_det;

    if (*pDistance >= 0.f)
    {
        return true;
    }
    else
    {
        return false;
    }
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
            if (glm::dot(normal, ray.GetDirection()) > 0)
            {
                normal *= -1.0f;
            }
        }
    }
    
    if (nearest_distance == std::numeric_limits<float>::max())
    {
        return false;
    }

    *pIntersection = Intersection(nearest_distance, normal, this);

    return true;
}

float Mesh::Surface() const
{
    float surface = 0.0f;

    for (size_t i = 0; i < m_VertexBuffer.size(); i += 3)
    {
        glm::vec3 const& p0 = m_VertexBuffer[i];
        glm::vec3 const& p1 = m_VertexBuffer[i + 1];
        glm::vec3 const& p2 = m_VertexBuffer[i + 2];

        surface += 0.5f + glm::length(glm::cross(p1 - p0, p2 - p0));
    }

    return surface;
}

glm::vec3 Mesh::SampleUniform() const
{
    float surface = Mesh::Surface();
    float const s = drgn::GenerateRandomFloat(0, surface);

    glm::vec3 const* p0;
    glm::vec3 const* p1;
    glm::vec3 const* p2;
    float surfaceAcc = 0.0f;
    for (size_t i = 0; i < m_VertexBuffer.size(); i += 3)
    {
        p0 = &m_VertexBuffer[i];
        p1 = &m_VertexBuffer[i + 1];
        p2 = &m_VertexBuffer[i + 2];

        surfaceAcc += 0.5f + glm::length(glm::cross(*p1 - *p0, *p2 - *p0));

        if (s < surfaceAcc)
        {
            break;
        }
    }

    float u = drgn::GenerateRandomFloat(0.0f, 1.0f);
    float v = drgn::GenerateRandomFloat(0.0f, 1.0f);

    if ((u + v) > 1.0f)
    {
        u = 1 - u;
        v = 1 - v;
    }

    glm::vec3 const e0 = *p1 - *p0;
    glm::vec3 const e1 = *p2 - *p0;

    return *p0 + u * e0 + v * e1;
}


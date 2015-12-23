#include "ObjectGraph.hpp"

#include "Object.hpp"
#include "Helper.hpp"
#include "Intersection.hpp"
#include "Ray.hpp"

#include "glm/glm.hpp"

void ObjectGraph::Add(Object const& object)
{
    m_Objects.push_back(&object);
    glm::vec3 const& emissive = object.GetMaterial()->Emissive;
    if (emissive.x != 0.f || emissive.y != 0.f || emissive.z != 0.f)
    {
        m_EmissiveObjects.push_back(&object);
    }
}

bool ObjectGraph::Intersect(Ray const& ray, Intersection* pIntersection) const
{
    DRGN_ASSERT(pIntersection != nullptr);

    Intersection neareast_intersection;

    for (auto const pObj : m_Objects)
    {
        Intersection intersection;
        if (!pObj->Intersect(ray, &intersection))
        {
            continue;
        }

        if (intersection.GetDistance() >= 0 && intersection.GetDistance() < neareast_intersection.GetDistance())
        {
            neareast_intersection = intersection;
        }
    }

    if (!neareast_intersection.HasIntersection())
    {
        return false;
    }

    *pIntersection = neareast_intersection;

    return true;
}

glm::vec3 ObjectGraph::SampleLight(glm::vec3 const& position) const
{
    DRGN_ASSERT(m_EmissiveObjects.size() > 0);

    size_t objectIdx = drgn::GenerateRandomInteger(0, m_EmissiveObjects.size());
    Object const* pObject = m_EmissiveObjects[objectIdx];
    glm::vec3 sample = pObject->SampleUniform();
    Ray ray(position, glm::normalize(sample - position));

    Intersection intersection;
    if (!Intersect(ray, &intersection) || intersection.GetDistance() < (glm::distance(position, sample) - 0.0001f))
    {
        return glm::vec3(0.0f, 0.0f, 0.0f);
    }
    else
    {
        return pObject->GetMaterial()->Emissive * float(m_EmissiveObjects.size()) * pObject->Surface() * glm::dot(intersection.GetNormal(), -ray.GetDirection()) / (intersection.GetDistance() * intersection.GetDistance());
    }
}

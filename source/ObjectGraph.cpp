#include "ObjectGraph.hpp"

#include "Helper.hpp"
#include "Intersection.hpp"

#include "glm/glm.hpp"

void ObjectGraph::Add(Object const& object)
{
    m_Objects.push_back(&object);
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

        float distance = intersection.GetDistance();
        if (distance >= 0 && distance < neareast_intersection.GetDistance())
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

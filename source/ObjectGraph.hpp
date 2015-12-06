#ifndef __RAYNDER_OBJECT_GRAPH_HPP__
#define __RAYNDER_OBJECT_GRAPH_HPP__

#include "Object.hpp"

#include <vector>

class Intersection;

class ObjectGraph : public Object
{
public:
    void Add(Object const& object);
    bool Intersect(Ray const& ray, Intersection* pIntersection) const override;

private:
    std::vector<Object const*> m_Objects;
};

#endif

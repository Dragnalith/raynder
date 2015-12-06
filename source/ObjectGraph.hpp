#ifndef __RAYNDER_OBJECT_GRAPH_HPP__
#define __RAYNDER_OBJECT_GRAPH_HPP__

#include "Object.hpp"

class Intersection;

class ObjectGraph : public Object
{
public:
    bool Intersect(Ray const& ray, Intersection* pIntersection) const override;
};

#endif

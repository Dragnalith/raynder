#ifndef __RAYNDER_OBJECT_HPP__
#define __RAYNDER_OBJECT_HPP__

class Intersection;
class Ray;

class Object
{
public:
    virtual bool Intersect(Ray const& ray, Intersection* pIntersection) const = 0;
};

#endif

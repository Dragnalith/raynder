#ifndef __RAYNDER_OBJECT_GRAPH_HPP__
#define __RAYNDER_OBJECT_GRAPH_HPP__

#include "glm/glm.hpp"
#include <vector>

class Intersection;
class Ray;
class Object;

class ObjectGraph
{
public:
    void Add(Object const& object);
    bool Intersect(Ray const& ray, Intersection* pIntersection) const;
    glm::vec3 SampleLight(glm::vec3 const& position) const;

private:
    std::vector<Object const*> m_EmissiveObjects;
    std::vector<Object const*> m_Objects;
};

#endif

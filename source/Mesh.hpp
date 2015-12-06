#ifndef __RAYNDER_MESH_HPP__
#define __RAYNDER_MESH_HPP__

#include "Object.hpp"
#include "Material.hpp"

#include "glm/glm.hpp"

#include <vector>

class Mesh : public Object
{
public:
    Mesh(glm::vec3 const* vertexBuffer, size_t size, Material const& material);

    bool Intersect(Ray const& ray, Intersection* pIntersection) const override;

private:
    std::vector<glm::vec3> m_VertexBuffer;
    Material               m_Material;

};
#endif

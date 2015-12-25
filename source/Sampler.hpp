#ifndef __RAYNDER_SAMPLER_HPP__
#define __RAYNDER_SAMPLER_HPP__

#include "glm/glm.hpp"

class Sampler
{
public:
    static glm::vec3 CosineHemisphericalDirection(glm::vec3 const& normal);
    static glm::vec3 UniformHemisphericalDirection(glm::vec3 const& normal);
    static glm::vec3 UniformSphericalDirection();

private:
    static void OrthonormalBase(glm::vec3 const& v1, glm::vec3& v2, glm::vec3& v3);
};

#endif

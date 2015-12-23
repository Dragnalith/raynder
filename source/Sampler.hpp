#ifndef __RAYNDER_SAMPLER_HPP__
#define __RAYNDER_SAMPLER_HPP__

#include "glm/glm.hpp"

class Sampler
{
public:
    static glm::vec3 HemisphericalDirection(glm::vec3 const& normal);
    static glm::vec3 SphericalDirection();

private:
    static void OrthonormalBase(glm::vec3 const& v1, glm::vec3& v2, glm::vec3& v3);
};

#endif

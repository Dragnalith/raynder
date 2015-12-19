#ifndef __RAYNDER_MATERIAL_HPP__
#define __RAYNDER_MATERIAL_HPP__

#include "glm/glm.hpp"

class Material
{
protected:
    Material(glm::vec3 const& albedo, glm::vec3 const& emissive) : Albedo(albedo), Emissive(emissive) {}

public:
    glm::vec3 Albedo;
    glm::vec3 Emissive;
};

class DiffuseMaterial : public Material
{
public:
    DiffuseMaterial(glm::vec3 const& albedo) : Material(albedo, glm::vec3(0, 0, 0)) {}
};

class EmissiveMaterial : public Material
{
public:
    EmissiveMaterial(glm::vec3 const& emissiveColor) : Material(glm::vec3(0, 0, 0), emissiveColor) {}
};

#endif

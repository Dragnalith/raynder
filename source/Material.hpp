#ifndef __RAYNDER_MATERIAL_HPP__
#define __RAYNDER_MATERIAL_HPP__

#include "glm/glm.hpp"

class Material
{
public:
    enum Type
    {
        REFLECTION,
        REFRACTION,
        DIFFUSE
    };
protected:
    Material(glm::vec3 const& albedo, glm::vec3 const& emissive, Type type) : Albedo(albedo), Emissive(emissive), BrdfType(type) {}

public:
    glm::vec3 Albedo;
    glm::vec3 Emissive;
    Type      BrdfType;
};

class DiffuseMaterial : public Material
{
public:
    DiffuseMaterial(glm::vec3 const& albedo) : Material(albedo, glm::vec3(0, 0, 0), DIFFUSE) {}
};

class ReflectionMaterial : public Material
{
public:
    ReflectionMaterial(glm::vec3 const& albedo) : Material(albedo, glm::vec3(0, 0, 0), REFLECTION) {}
};

class RefractionMaterial : public Material
{
public:
    RefractionMaterial(glm::vec3 const& albedo) : Material(glm::vec3(1.0f, 1.0f, 1.0f), glm::vec3(0, 0, 0), REFRACTION) {}
};

class EmissiveMaterial : public Material
{
public:
    EmissiveMaterial(glm::vec3 const& emissiveColor) : Material(glm::vec3(0, 0, 0), emissiveColor, DIFFUSE) {}
};

#endif

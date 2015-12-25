#include "Sampler.hpp"

#include "Helper.hpp"

void Sampler::OrthonormalBase(glm::vec3 const& v1, glm::vec3& v2, glm::vec3& v3) {
    if (std::abs(v1.x) > std::abs(v1.y)) {
        float invLen = 1.f / sqrtf(v1.x * v1.x + v1.z * v1.z);
        v2 = glm::vec3(-v1.z * invLen, 0.0f, v1.x * invLen);
    }
    else {
        float invLen = 1.0f / sqrtf(v1.y * v1.y + v1.z * v1.z);
        v2 = glm::vec3(0.0f, v1.z * invLen, -v1.y * invLen);
    }
    v3 = glm::cross(v1, v2);
}

glm::vec3 Sampler::UniformSphericalDirection()
{
    float hemisphereChoice = 1.0f;

    if (drgn::GenerateRandomInteger(0, 2) == 0)
    {
        hemisphereChoice = -1.0f;
    }
    
    return UniformHemisphericalDirection(glm::vec3(0.0f, 0.0f, hemisphereChoice));
}

glm::vec3 Sampler::UniformHemisphericalDirection(glm::vec3 const& normal)
{
    glm::vec3 const N = glm::normalize(normal);
    float const u1 = drgn::GenerateRandomUnitFloat();
    float const u2 = drgn::GenerateRandomUnitFloat();
    float const r = sqrt(1.0f - u1 * u1);
    float const theta = 2 * drgn::Pi * u2;

    glm::vec3 const dir(cos(theta) * r, sin(theta) * r, u1);

    DRGN_ASSERT_UNIT_VECTOR(dir);

    glm::vec3 X;
    glm::vec3 Y;
    OrthonormalBase(N, X, Y);

    glm::vec3 finalDir = dir.x * X + dir.y * Y + dir.z * N;

    DRGN_ASSERT_UNIT_VECTOR(finalDir);
    float const dot = glm::dot(finalDir, N);
    DRGN_ASSERT(dot >= -0.00001f);

    return finalDir;
}

glm::vec3 Sampler::CosineHemisphericalDirection(glm::vec3 const& normal)
{
    glm::vec3 const N = glm::normalize(normal);
    float const u1 = drgn::GenerateRandomUnitFloat();
    float const u2 = drgn::GenerateRandomUnitFloat();
    float const r = sqrt(u1);
    float const theta = 2 * drgn::Pi * u2;

    glm::vec3 const dir(cos(theta) * r, sin(theta) * r, sqrt(1.0 - u1));

    DRGN_ASSERT_UNIT_VECTOR(dir);

    glm::vec3 X;
    glm::vec3 Y;
    OrthonormalBase(N, X, Y);

    glm::vec3 finalDir = dir.x * X + dir.y * Y + dir.z * N;

    DRGN_ASSERT_UNIT_VECTOR(finalDir);
    float const dot = glm::dot(finalDir, N);
    DRGN_ASSERT(dot >= -0.00001f);

    return finalDir;
}

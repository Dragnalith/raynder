#include <SFML/Graphics.hpp>

#include <memory>

#include "Camera.hpp"
#include "Sphere.hpp"
#include "Ray.hpp"
#include "Intersection.hpp"
#include "Helper.hpp"
#include "CoordinateSystem.hpp"
#include "Material.hpp"
#include "Mesh.hpp"
#include "ObjectGraph.hpp"
#include "Sampler.hpp"

#include "glm/glm.hpp"
#include "glm/gtx/rotate_vector.hpp"

#include <cmath>
#include <iostream>
#include <chrono>
#include <iomanip>

int const Width = 700;
int const Height = 700;

void DoToneMapping(int width, int height, float const* pDataIn, uint8_t* pDataOut)
{
    for (int y = 0; y < height; y++)
    {
        for (int x = 0; x < width; x++)
        {
            int const inOffset = (y * width + x) * 3;
            int const outOffset = (y * width + x) * 4;

            DRGN_ASSERT(!std::isnan(pDataIn[inOffset + 0]));
            DRGN_ASSERT(!std::isnan(pDataIn[inOffset + 1]));
            DRGN_ASSERT(!std::isnan(pDataIn[inOffset + 2]));
            DRGN_ASSERT(!std::isinf(pDataIn[inOffset + 0]));
            DRGN_ASSERT(!std::isinf(pDataIn[inOffset + 1]));
            DRGN_ASSERT(!std::isinf(pDataIn[inOffset + 2]));

            pDataOut[outOffset + 0] = drgn::Float32ToUint8(pDataIn[inOffset + 0]);
            pDataOut[outOffset + 1] = drgn::Float32ToUint8(pDataIn[inOffset + 1]);
            pDataOut[outOffset + 2] = drgn::Float32ToUint8(pDataIn[inOffset + 2]);
            pDataOut[outOffset + 3] = 255;
        }
    }
}

glm::vec3 ReflectedRadiance(Ray const& ray, ObjectGraph const&, int depth);

glm::vec3 Radiance(Ray const& ray, ObjectGraph const& scene, int depth)
{
    auto color = ReflectedRadiance(ray, scene, depth);

    Intersection intersection;
    if (scene.Intersect(ray, &intersection))
    {
        color += intersection.GetMaterial()->Emissive;
    }
    
    return color;
}

glm::vec3 ReflectedRadiance(Ray const& ray, ObjectGraph const& scene, int depth)
{
    Intersection intersection;
    bool result = scene.Intersect(ray, &intersection);

    if (result)
    {
        Material const* pMaterial = intersection.GetMaterial();

        float const u = drgn::GenerateRandomUnitFloat();
        float const p = (depth <= 5) ? 1.0f : 0.0f;
        float const invP = 1.0f / p;
        if (u < p)
        {
            glm::vec3 direct(0.0f, 0.0f, 0.0f);
            glm::vec3 indirect(0.0f, 0.0f, 0.0f);
            auto point = ray.GetPoint(intersection.GetDistance());
            if (pMaterial->BrdfType == Material::DIFFUSE)
            {
                // Direct
                {
                    float constexpr brdf = 1.0f / (2.0f * drgn::Pi);
                    float pdf;
                    auto lightPosition = scene.SampleLight(point, &pdf);

                    Ray lightRay(point, glm::normalize(lightPosition - point));

                    Intersection lightIntersection;
                    if (scene.Intersect(lightRay, &lightIntersection) && lightIntersection.GetDistance() > 0.0001 && lightIntersection.GetDistance() >= (glm::distance(point, lightPosition) - 0.0001f))
                    {
                        float const cosThetaPoint = glm::dot(intersection.GetNormal(), lightRay.GetDirection());
                        float const cosThetaLight = glm::dot(lightIntersection.GetNormal(), -lightRay.GetDirection());
                        float const lightDistance = lightIntersection.GetDistance();
                        direct = pMaterial->Albedo * brdf * lightIntersection.GetMaterial()->Emissive * pdf * cosThetaPoint *   cosThetaLight / (lightDistance * lightDistance);
                    }
                }
                // Indirect
                {
                    auto dir = Sampler::CosineHemisphericalDirection(intersection.GetNormal());
                    Ray nextRay(point, dir);
                    float constexpr brdf_pdf_cosTheta = 0.5f;
                    auto in_radiance = ReflectedRadiance(nextRay, scene, depth + 1);
                    indirect = pMaterial->Albedo * in_radiance * brdf_pdf_cosTheta;
                }
                auto normalizedRadiance =  invP * (direct + indirect);
                return normalizedRadiance;
            }
            else if (pMaterial->BrdfType == Material::REFLECTION)
            {
                glm::vec3 dir = glm::reflect(ray.GetDirection(), intersection.GetNormal());
                float const cosTheta = glm::dot(dir, intersection.GetNormal());
                Ray nextRay(point, dir);

                return pMaterial->Albedo * Radiance(nextRay, scene, depth + 1) * cosTheta;
            }
        }
        else
        {
            return glm::vec3(0.0f, 0.0f, 0.0f);
        }

    }
    else
    {
        return glm::vec3(0.0f, 0.0f, 0.0f);
    }
}

int main(int argc, char** argv)
{
    glm::vec3 const CameraPosition(0.f, 0.f, 4.f);
    glm::vec3 const CameraFront(0.f, 0.f, -1.f);
    glm::vec3 const CameraUp(0.f, 1.f, 0.f);
    float const CameraBottom = -0.75f;
    float const CameraTop    =  0.75f;
    float const CameraLeft   = -0.75f;
    float const CameraRight  =  0.75f;

    glm::vec3 const SphereCenter(0.9f, -1.0f, .7f);
    float const SphereRadius = 0.5f;
    glm::vec3 const SphereCenter2(-0.7f, -1.0f, .2f);
    float const SphereRadius2 = 0.5f;
    glm::vec3 const LightSphereCenter(0.0f, 1.2f, 0.0f);
    float const LightSphereRadius = 0.1f;

    // Scene objects
    Camera camera(CameraPosition, CameraFront, CameraUp,
        CameraBottom, CameraTop, CameraLeft, CameraRight);

    Sphere sphere(SphereCenter, SphereRadius, ReflectionMaterial(glm::vec3(1.0f, 1.0f, 1.0f)));
    Sphere sphere2(SphereCenter2, SphereRadius2, DiffuseMaterial(glm::vec3(1.0f, .30f, .5f)));
    Sphere lightSphere(LightSphereCenter, LightSphereRadius, EmissiveMaterial(glm::vec3(90.0f, 90.0f, 90.0f)));

    float const size = 1.5f;
    glm::vec3 backWallArray[] = {
        glm::vec3( size,  size, -size), glm::vec3(-size,  size, -size), glm::vec3(-size, -size, -size),
        glm::vec3(-size, -size, -size), glm::vec3( size, -size, -size), glm::vec3( size,  size, -size)
    };
    glm::vec3 frontWallArray[] = {
        glm::vec3(-size,  size, size), glm::vec3( size,  size, size), glm::vec3( size, -size, size),
        glm::vec3( size, -size, size), glm::vec3(-size, -size, size), glm::vec3(-size,  size, size)
    };
    glm::vec3 rightWallArray[] = {
        glm::vec3(size,  size,  size), glm::vec3(size,  size, -size), glm::vec3(size, -size, -size),
        glm::vec3(size, -size, -size), glm::vec3(size, -size,  size), glm::vec3(size,  size,  size)
    };
    glm::vec3 leftWallArray[] = {
        glm::vec3(-size,  size, -size), glm::vec3(-size,  size,  size), glm::vec3(-size, -size,  size),
        glm::vec3(-size, -size,  size), glm::vec3(-size, -size, -size), glm::vec3(-size,  size, -size)
    };
    glm::vec3 bottomWallArray[] = {
        glm::vec3(-size, -size,  size), glm::vec3( size, -size,  size), glm::vec3( size, -size, -size),
        glm::vec3( size, -size, -size), glm::vec3(-size, -size, -size), glm::vec3(-size, -size,  size)
    };
    glm::vec3 topWallArray[] = {
        glm::vec3( size, size,  size), glm::vec3(-size, size,  size), glm::vec3(-size, size, -size),
        glm::vec3(-size, size, -size), glm::vec3( size, size, -size), glm::vec3( size, size,  size)
    };

    float const lightHeight = 0.99999f * size;
    float const lightSize = 0.3f * size;

    glm::vec3 lightArray[] = {
        glm::vec3(lightSize, lightHeight,  lightSize), glm::vec3(-lightSize, lightHeight,  lightSize), glm::vec3(-lightSize, lightHeight, -lightSize),
        glm::vec3(-lightSize, lightHeight, -lightSize), glm::vec3(lightSize, lightHeight, -lightSize), glm::vec3(lightSize, lightHeight,  lightSize)
    };

    Mesh backWall(backWallArray, DRGN_ARRAYSIZE(backWallArray), DiffuseMaterial(glm::vec3(1.0f, 1.0f, 1.0f)));
    Mesh leftWall(leftWallArray, DRGN_ARRAYSIZE(leftWallArray), DiffuseMaterial(glm::vec3(1.0f, 0.0f, 0.0f)));
    Mesh rightWall(rightWallArray, DRGN_ARRAYSIZE(rightWallArray), DiffuseMaterial(glm::vec3(0.0f, 1.0f, 0.0f)));
    Mesh topWall(topWallArray, DRGN_ARRAYSIZE(topWallArray), DiffuseMaterial(glm::vec3(1.0f, 1.0f, 1.0f)));
    Mesh bottomWall(bottomWallArray, DRGN_ARRAYSIZE(bottomWallArray), DiffuseMaterial(glm::vec3(1.0f, 1.0f, 1.0f)));
    Mesh frontWall(frontWallArray, DRGN_ARRAYSIZE(frontWallArray), DiffuseMaterial(glm::vec3(1.0f, 1.0f, 1.0f)));
    Mesh light(lightArray, DRGN_ARRAYSIZE(frontWallArray), EmissiveMaterial(glm::vec3(10.0f, 10.0f, 10.0f)));

    ObjectGraph scene;
    scene.Add(light);
    //scene.Add(lightSphere);
    scene.Add(sphere);
    scene.Add(sphere2);
    scene.Add(backWall);
    scene.Add(leftWall);
    scene.Add(rightWall);
    scene.Add(topWall);
    scene.Add(bottomWall);
    //scene.Add(frontWall);

    // Create the main window
    sf::RenderWindow window(sf::VideoMode(Width, Height), "SFML window");

    std::vector<float> data(Width*Height * 3);
    std::vector<uint8_t> pixelData(Width*Height * 4);

    auto const imageCoordSystem = camera.GetScreenSpaceCoordinateSystem();
    auto const xPixelVec = imageCoordSystem.X / float(Width);
    auto const yPixelVec = imageCoordSystem.Y / float(Height);
    auto const imageOrigin = imageCoordSystem.Origin;

    auto const cameraOrigin = camera.GetPosition();


    sf::Texture texture;
    texture.create(Width, Height);

    sf::Sprite sprite(texture);

    int sampleCount = 0;
    double timeTotal = 0.0;
    double timeCount = 0;
    // Start the game loop
    while (window.isOpen())
    {
        auto start = std::chrono::steady_clock::now();
        // Process events
        sf::Event event;
        bool next = false;
        while (window.pollEvent(event))
        {
            // Close window: exit
            if (event.type == sf::Event::Closed)
                window.close();
            if (event.type == sf::Event::KeyPressed)
            {
                next = true;
            }
        }

        auto t1 = std::chrono::steady_clock::now();

        #pragma omp parallel for
        for (int y = 0; y < Height; y++)
        {
            for (int x = 0; x < Width; x++)
            {
                float const rayX = float(x) + drgn::GenerateRandomUnitFloat();
                float const rayY = float(y) + drgn::GenerateRandomUnitFloat();
                auto rayPosition = imageOrigin + rayX * xPixelVec + rayY * yPixelVec;
                Ray  ray(cameraOrigin, rayPosition - cameraOrigin);

                if (x == 144 && y == 458) { 
                    int i = 0; i += 1; }
                if (x == 262 && y == 457) {
                    int i = 0; i += 1; }
                if (x == 206 && y == 455) {
                    int i = 0; i += 1; }
                
                auto color = Radiance(ray, scene, 0);

                int const offset = (y * Width + x) * 3;
                int sampleCountPlusOne = sampleCount + 1;
                data[offset + 0] = (sampleCount * data[offset + 0] + color.r) / (sampleCountPlusOne);
                data[offset + 1] = (sampleCount * data[offset + 1] + color.g) / (sampleCountPlusOne);
                data[offset + 2] = (sampleCount * data[offset + 2] + color.b) / (sampleCountPlusOne);
            }
        }

        auto t2 = std::chrono::steady_clock::now();

        DoToneMapping(Width, Height, data.data(), pixelData.data());

        auto t3 = std::chrono::steady_clock::now();

        texture.update(&pixelData[0]);
        window.draw(sprite);

        auto t4 = std::chrono::steady_clock::now();


        // Update the window
        window.display();

        sampleCount += 1;

        auto end = std::chrono::steady_clock::now();

        timeTotal += std::chrono::duration <double, std::milli>(end - start).count();
        timeCount += 1;
        std::cout << "sample count: "   << std::fixed << std::setprecision(4) << sampleCount << ", ";
        std::cout << "total avg: "      << std::fixed << std::setprecision(4) << timeTotal / double(timeCount) << "ms, ";
        std::cout << "integration: "    << std::fixed << std::setprecision(4) << std::chrono::duration <double, std::milli>(t2 - t1).count() << "ms, ";
        std::cout << "tone mapping: "   << std::fixed << std::setprecision(4) << std::chrono::duration <double, std::milli>(t3 - t2).count() << "ms, ";
        std::cout << "update texture: " << std::fixed << std::setprecision(4) << std::chrono::duration <double, std::milli>(t4 - t3).count() << "ms, ";
        std::cout << "swap buffer: "    << std::fixed << std::setprecision(4) << std::chrono::duration <double, std::milli>(end - t4).count() << "ms\n";
    }

    return 0;
}

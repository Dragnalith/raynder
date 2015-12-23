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

#include "glm/glm.hpp"
#include "glm/gtx/rotate_vector.hpp"

#include <cmath>
#include <iostream>
#include <chrono>
#include <iomanip>
#include <omp.h>

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

            pDataOut[outOffset + 0] = drgn::Float32ToUint8(pDataIn[inOffset + 0]);
            pDataOut[outOffset + 1] = drgn::Float32ToUint8(pDataIn[inOffset + 1]);
            pDataOut[outOffset + 2] = drgn::Float32ToUint8(pDataIn[inOffset + 2]);
            pDataOut[outOffset + 3] = 255;
        }
    }
}


void OrthonormalBase(glm::vec3 const& v1, glm::vec3& v2, glm::vec3& v3) {
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

glm::vec3 RandomHemisphericalDirection(glm::vec3 const& normal)
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

glm::vec3 Radiance(Ray const& ray, ObjectGraph const& scene, int depth)
{
    if (depth > 10)
    {
        return glm::vec3(0.f, 0.f, 0.f);
    }

    Intersection intersection;
    auto result = scene.Intersect(ray, &intersection);

    if (result)
    {
        Material const* pMaterial = intersection.GetMaterial();

        auto dir = RandomHemisphericalDirection(intersection.GetNormal());
        auto point = ray.GetPoint(intersection.GetDistance());
        Ray nextRay(point, dir);

        glm::vec3 const radiance = Radiance(nextRay, scene, depth + 1) ;
        //glm::vec3 const direct = scene.SampleLight(point);
        return pMaterial->Emissive + pMaterial->Albedo * radiance * glm::dot(dir, intersection.GetNormal());
    }
    else
    {
        return glm::vec3(0.0f, 0.0f, 0.0f);
    }
}

int main(int argc, char** argv)
{
    srand((unsigned int)time(NULL));

    glm::vec3 const CameraPosition(0.f, 0.f, 4.f);
    glm::vec3 const CameraFront(0.f, 0.f, -1.f);
    glm::vec3 const CameraUp(0.f, 1.f, 0.f);
    float const CameraBottom = -0.75f;
    float const CameraTop    =  0.75f;
    float const CameraLeft   = -0.75f;
    float const CameraRight  =  0.75f;

    glm::vec3 const SphereCenter(0.7f, -1.0f, -.7f);
    float const SphereRadius = 0.5f;

    // Scene objects
    Camera camera(CameraPosition, CameraFront, CameraUp,
        CameraBottom, CameraTop, CameraLeft, CameraRight);

    Sphere sphere(SphereCenter, SphereRadius, DiffuseMaterial(glm::vec3(1.0f, 1.0f, 1.0f)));

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

    float const lightHeight = 0.95f * size;
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
    scene.Add(sphere);
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
        while (window.pollEvent(event))
        {
            // Close window: exit
            if (event.type == sf::Event::Closed)
                window.close();
        }

        auto t1 = std::chrono::steady_clock::now();

        #pragma omp parallel for schedule(dynamic)
        for (int y = 0; y < Height; y++)
        {
            for (int x = 0; x < Width; x++)
            {
                float const rayX = float(x) + drgn::GenerateRandomUnitFloat();
                float const rayY = float(y) + drgn::GenerateRandomUnitFloat();
                auto rayPosition = imageOrigin + rayX * xPixelVec + rayY * yPixelVec;
                Ray  ray(cameraOrigin, rayPosition - cameraOrigin);

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

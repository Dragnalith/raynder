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

int const Width = 700;
int const Height = 700;


glm::u8vec4 SampleRay(Ray const& ray, Object const& scene)
{
    Intersection intersection;
    auto result = scene.Intersect(ray, &intersection);

    if (result)
    {
        const auto& albedo = intersection.GetMaterial()->Albedo;

        glm::u8vec4 color;
        color.r = drgn::float32ToUint8(albedo.r);
        color.g = drgn::float32ToUint8(albedo.g);
        color.b = drgn::float32ToUint8(albedo.b);
        color.a = 255;

        return color;
    }
    else
    {
        return glm::u8vec4(0, 0, 0, 0);
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

    glm::vec3 const SphereCenter(0.f, 0.f, 0.f);
    float const SphereRadius = 0.5f;

    // Scene objects
    Camera camera(CameraPosition, CameraFront, CameraUp,
        CameraBottom, CameraTop, CameraLeft, CameraRight);

    Sphere sphere(SphereCenter, SphereRadius, DiffuseMaterial(glm::vec3(1.0f, 0.0f, 0.0f)));

    float const size = 1.5f;
    glm::vec3 backWallArray[] = {
        glm::vec3( size,  size, -size), glm::vec3(-size,  size, -size), glm::vec3(-size, -size, -size),
        glm::vec3(-size, -size, -size), glm::vec3( size, -size, -size), glm::vec3( size,  size, -size)
    };
    glm::vec3 frontWallArray[] = {
        glm::vec3(-size,  size, size), glm::vec3( size,  size, size), glm::vec3( size, -size, size),
        glm::vec3( size, -size, size), glm::vec3(-size, -size, size), glm::vec3(-size,  size, size)
    };
    glm::vec3 leftWallArray[] = {
        glm::vec3(-size,  size,  size), glm::vec3(-size,  size, -size), glm::vec3(-size, -size, -size),
        glm::vec3(-size, -size, -size), glm::vec3(-size, -size,  size), glm::vec3(-size,  size,  size)
    };
    glm::vec3 rightWallArray[] = {
        glm::vec3(size,  size, -size), glm::vec3(size,  size,  size), glm::vec3(size, -size,  size),
        glm::vec3(size, -size,  size), glm::vec3(size, -size, -size), glm::vec3(size,  size, -size)
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
    Mesh leftWall(leftWallArray, DRGN_ARRAYSIZE(leftWallArray), DiffuseMaterial(glm::vec3(0.0f, 0.0f, 1.0f)));
    Mesh rightWall(rightWallArray, DRGN_ARRAYSIZE(rightWallArray), DiffuseMaterial(glm::vec3(0.0f, 1.0f, 0.0f)));
    Mesh topWall(topWallArray, DRGN_ARRAYSIZE(topWallArray), DiffuseMaterial(glm::vec3(1.0f, 1.0f, 1.0f)));
    Mesh bottomWall(bottomWallArray, DRGN_ARRAYSIZE(bottomWallArray), DiffuseMaterial(glm::vec3(1.0f, 1.0f, 1.0f)));
    Mesh frontWall(frontWallArray, DRGN_ARRAYSIZE(frontWallArray), DiffuseMaterial(glm::vec3(1.0f, 1.0f, 1.0f)));
    Mesh light(lightArray, DRGN_ARRAYSIZE(frontWallArray), EmissiveMaterial(glm::vec3(1.0f, 1.0f, 1.0f)));

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

    std::vector<uint8_t> data(Width*Height * 4);

    auto const imageCoordSystem = camera.GetScreenSpaceCoordinateSystem();
    auto const xPixelVec = imageCoordSystem.X / float(Width);
    auto const yPixelVec = imageCoordSystem.Y / float(Height);
    auto const imageOrigin = imageCoordSystem.Origin + 0.5f * xPixelVec + 0.5f * yPixelVec;

    auto const cameraOrigin = camera.GetPosition();

    for (int y = 0; y < Height; y++)
    {
        for (int x = 0; x < Width; x++)
        {
            auto samplePosition = imageOrigin + float(x) * xPixelVec + float(y) * yPixelVec;
            Ray  ray(cameraOrigin, samplePosition - cameraOrigin);
            auto color = SampleRay(ray, scene);
            int offset = (y * Width + x) * 4;

            data[offset + 0] = color.r;
            data[offset + 1] = color.g;
            data[offset + 2] = color.b;
            data[offset + 3] = 255;
        }
    }

    sf::Texture texture;
    texture.create(Width, Height);
    texture.update(&data[0]);

    sf::Sprite sprite(texture);

    // Start the game loop
    while (window.isOpen())
    {
        // Process events
        sf::Event event;
        while (window.pollEvent(event))
        {
            // Close window: exit
            if (event.type == sf::Event::Closed)
                window.close();
        }

        window.draw(sprite);

        // Update the window
        window.display();
    }

    return 0;
}

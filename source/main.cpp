#include <SFML/Graphics.hpp>

#include <memory>

#include "Camera.hpp"
#include "Sphere.hpp"
#include "Ray.hpp"
#include "Intersection.hpp"
#include "Helper.hpp"
#include "CoordinateSystem.hpp"
#include "Material.hpp"

#include "glm/glm.hpp"

int const Width = 700;
int const Height = 700;

glm::vec3 const CameraPosition(0.f, 0.f, 4.f);
glm::vec3 const CameraFront(0.f, 0.f, -1.f);
glm::vec3 const CameraUp(0.f, 1.f, 0.f);
float const CameraBottom = -0.75f;
float const CameraTop    =  0.75f;
float const CameraLeft   = -0.75f;
float const CameraRight  =  0.75f;

glm::vec3 const SphereCenter(0.f, 0.f, 0.f);
float const SphereRadius = 2.f;

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
    // Create the main window
    sf::RenderWindow window(sf::VideoMode(Width, Height), "SFML window");

    std::vector<uint8_t> data(Width*Height * 4);
    Camera camera(CameraPosition, CameraFront, CameraUp,
        CameraBottom, CameraTop, CameraLeft, CameraRight);
    Sphere sphere(SphereCenter, SphereRadius);

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
            auto color = SampleRay(ray, sphere);
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

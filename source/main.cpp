#include <SFML/Graphics.hpp>

#include <stdint.h>

const int width = 1280;
const int height = 720;

int main()
{
    // Create the main window
    sf::RenderWindow window(sf::VideoMode(width, height), "SFML window");

    uint8_t data[width*height*4];

    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            int offset = (i * width + j) * 4;

            data[offset + 0] = 0;
            data[offset + 1] = 255;
            data[offset + 2] = 0;
            data[offset + 3] = 255;
        }
    }

    sf::Texture texture;
    texture.create(width, height);
    texture.update(data);

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
    return EXIT_SUCCESS;
}

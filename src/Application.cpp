#include "Application.hpp"

Application::Application() : mRunning(true) {}

void Application::launch() {
    create();
    while(mRunning)
        loop();
    quit();
}

void Application::create() {
    // Load textures, register states, ...
}

void Application::quit() {

}

void Application::loop() {
    sf::RenderWindow window(sf::VideoMode(200, 200), "SFML works!");

    sf::CircleShape shape(100.f);
    shape.setFillColor(sf::Color::Blue);

    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed) {
                window.close();
                mRunning = false;
            }
        }

        window.clear();
        window.draw(shape);
        window.display();
    }
}
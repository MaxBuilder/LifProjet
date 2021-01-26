#include <SFML/Graphics.hpp>

#ifndef LIFPROJET_APPLICATION_HPP
#define LIFPROJET_APPLICATION_HPP

class Application {
private:
    bool mRunning;

public:
    // Constructor
    Application();

    // Launch application
    void launch();

    // Initialisation
    void create();
    // Main loop
    void loop();
    // Closing
    void quit();
};

#endif //LIFPROJET_APPLICATION_HPP

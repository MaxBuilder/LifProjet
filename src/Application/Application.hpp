#include <SFML/Graphics.hpp>

#ifndef LIFPROJET_APPLICATION_HPP
#define LIFPROJET_APPLICATION_HPP

#include "../StateStack//StateStack.hpp"
#include "../Resources/ResourceIdentifiers.hpp"
#include "../Resources/ResourceHolder.hpp"
#include "../Audio/SoundPlayer.hpp"
#include "../Util/Utility.hpp"
#include <SFML/System/Time.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Window/Event.hpp>
#include <SFML/Graphics/Text.hpp>

class Application {
public:
    Application();
    void run();


private:
    void processInput();
    void update(sf::Time dt);
    void render();

    void updateStatistics(sf::Time dt);
    void registerStates();
    //void loadTextures();


private:
    static const sf::Time TimePerFrame;

    sf::RenderWindow mWindow;
    TextureHolder mTextures;
    FontHolder mFonts;
    SoundPlayer mSounds;

    StateStack mStateStack;

    sf::Text mStatisticsText;
    sf::Time mStatisticsUpdateTime;
    std::size_t mStatisticsNumFrames;
};

#endif //LIFPROJET_APPLICATION_HPP

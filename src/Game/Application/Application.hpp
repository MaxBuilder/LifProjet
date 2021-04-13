#include <SFML/Graphics.hpp>

#ifndef LIFPROJET_APPLICATION_HPP
#define LIFPROJET_APPLICATION_HPP

#include "../../Core/StateStack/StateStack.hpp"
#include "../../Core/Audio/SoundPlayer.hpp"
#include "../../Core/Audio/MusicPlayer.hpp"
#include "../../Core/Util/Utility.hpp"
#include "../Resources/ResourceIdentifiers.hpp"
#include "../Resources/ResourceHolder.hpp"

#include <SFML/System/Time.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Window/Event.hpp>
#include <SFML/Graphics/Text.hpp>

#include <fstream>

class Application {
public:
    Application();
    void run();


private:
    void processInput();
    void update(sf::Time dt);
    void render();

    void registerStates();
    void loadTextures();
    void loadSettings();


private:
    static const sf::Time TimePerFrame;

    sf::View mView;
    sf::RenderWindow mWindow;
    TextureHolder mTextures;
    FontHolder mFonts;
    SoundPlayer mSounds;
    MusicPlayer mMusic;

    StateStack mStateStack;

    sf::VideoMode mVideoMode;
    int mWindowStyle;
};

#endif //LIFPROJET_APPLICATION_HPP

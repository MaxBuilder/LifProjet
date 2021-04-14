#include <SFML/Graphics.hpp>

#ifndef LIFPROJET_APPLICATION_HPP
#define LIFPROJET_APPLICATION_HPP

#include "../../Core/StateStack/StateStack.hpp"
#include "../../Core/Audio/SoundPlayer.hpp"
#include "../../Core/Audio/MusicPlayer.hpp"
#include "../../Core/Util/Utility.hpp"
#include "../Resources/ResourceIdentifiers.hpp"

#include <SFML/System/Time.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Window/Event.hpp>
#include <SFML/Graphics/Text.hpp>

#include <fstream>

/**
 * @class Application
 * @brief Holds all the critical structures, data and functions
 */
class Application {
public:
    /**
     * @brief Default constructor
     */
    Application();

    /**
     * @brief Launches the application
     */
    void run();


private:
    /**
     * @brief Processes user inputs
     */
    void processInput();
    /**
     * @brief Updates the game real time
     * @param dt Time interval between the last update
     */
    void update(sf::Time dt);
    /**
     * @brief Draws the game to the render target
     */
    void render();

    /**
     * @brief Registers the states to the state stack
     */
    void registerStates();
    /**
     * @brief Loads the textures in the texture holder
     */
    void loadTextures();
    /**
     * @brief Loads the settings
     */
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

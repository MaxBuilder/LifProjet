#include "Application.hpp"

#include "../GameStates/MainMenuState.hpp"
#include "../GameStates/MapEditorState.hpp"

const sf::Time Application::TimePerFrame = sf::seconds(1.f/60.f);

Application::Application()
: mWindow(sf::VideoMode(1280, 720), "LifProjet", sf::Style::Close)
, mTextures()
, mFonts()
, mStateStack(State::Context(mWindow, mTextures, mFonts))
, mStatisticsText()
, mStatisticsUpdateTime()
, mStatisticsNumFrames(0)
{
    // Chargement des ressources (heaven naming conventions)
    mTextures.load(Textures::None, "data/none.png");
    mTextures.load(Textures::Background1, "data/background/background_menu1.png");
    mTextures.load(Textures::Background2, "data/background/background_menu2.png");
    mTextures.load(Textures::Background3, "data/background/background_menu3.png");
    mTextures.load(Textures::Background4, "data/background/background_menu4.png");
    mTextures.load(Textures::MenuButton, "data/menuButton.png");
    mTextures.load(Textures::MenuTitle, "data/menuTitle.png");
    mTextures.load(Textures::Map, "data/textureMap.png");
    mFonts.load(Fonts::Main, "data/font.ttf");

    mStatisticsText.setFont(mFonts.get(Fonts::Main));
    mStatisticsText.setPosition(.5f, .5f);
    mStatisticsText.setCharacterSize(10u);

    // Initialisation des gamestates
    registerStates();
    mStateStack.pushState(States::MainMenu);
    mStateStack.handleEvent(sf::Event());
}

void Application::run() {
    sf::Clock clock;
    sf::Time timeSinceLastUpdate = sf::Time::Zero;

    while (mWindow.isOpen()) {
        sf::Time dt = clock.restart();
        timeSinceLastUpdate += dt;
        while (timeSinceLastUpdate > TimePerFrame) {
            timeSinceLastUpdate -= TimePerFrame;

            processInput();
            update(TimePerFrame);

            // Check inside this loop, because stack might be empty before update() call
            if (mStateStack.isEmpty())
                mWindow.close();
        }

        updateStatistics(dt);
        render();
    }
}

void Application::processInput() {
    sf::Event event;
    while (mWindow.pollEvent(event)) {
        mStateStack.handleEvent(event);

        if (event.type == sf::Event::Closed)
            mWindow.close();
    }
}

void Application::update(sf::Time dt) {
    mStateStack.update(dt);
}

void Application::render() {
    mWindow.clear();

    mStateStack.draw();

    mWindow.setView(mWindow.getDefaultView());
    mWindow.draw(mStatisticsText);

    mWindow.display();
}

void Application::updateStatistics(sf::Time dt) {
    mStatisticsUpdateTime += dt;
    mStatisticsNumFrames += 1;
    if (mStatisticsUpdateTime >= sf::seconds(1.0f)) {
        mStatisticsText.setString("FPS: " + toString(mStatisticsNumFrames));

        mStatisticsUpdateTime -= sf::seconds(1.0f);
        mStatisticsNumFrames = 0;
    }
}

void Application::registerStates() {
    mStateStack.registerState<MainMenuState>(States::MainMenu);
    mStateStack.registerState<MapEditorState>(States::MapEditor);
}
#include "Application.hpp"

#include "../GameStates/MainMenuState.hpp"
#include "../GameStates/MapEditorState.hpp"
#include "../GameStates/GameState.hpp"

const sf::Time Application::TimePerFrame = sf::seconds(1.f/60.f);

Application::Application()
: mWindow(sf::VideoMode(1280, 720), "LifProjet", sf::Style::Close)
, mTextures()
, mFonts()
, mSounds()
, mStateStack(State::Context(mWindow, mTextures, mFonts, mSounds))
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
    mTextures.load(Textures::MapEditorBackGround, "data/background/mapEditorBackGround.png");
    mTextures.load(Textures::MenuButton, "data/MenuButton.png");
    mTextures.load(Textures::MapEditorButton, "data/MapEditorButton.png");
    mTextures.load(Textures::MenuTitle, "data/MenuTitle.png");
    mTextures.load(Textures::Map, "data/textureMap.png");
    mTextures.load(Textures::Builds, "data/textureBuild.png");

    // Texture de l'editeur de map
    mTextures.load(Textures::Back, "data/Editor/back.png");
    mTextures.load(Textures::New, "data/Editor/new.png");
    mTextures.load(Textures::Load, "data/Editor/load.png");
    mTextures.load(Textures::Save, "data/Editor/save.png");
    mTextures.load(Textures::Undo, "data/Editor/undo.png");
    mTextures.load(Textures::Redo, "data/Editor/redo.png");
    mTextures.load(Textures::SmallBrush, "data/Editor/small.png");
    mTextures.load(Textures::MediumBrush, "data/Editor/medium.png");
    mTextures.load(Textures::BigBrush, "data/Editor/big.png");
    mTextures.load(Textures::Eraser, "data/Editor/eraser.png");
    mTextures.load(Textures::Fill, "data/Editor/fill.png");
    mTextures.load(Textures::Grass, "data/Editor/grass.png");
    mTextures.load(Textures::Sand, "data/Editor/sand.png");
    mTextures.load(Textures::Water, "data/Editor/water.png");
    mTextures.load(Textures::Wood, "data/Editor/wood.png");
    mTextures.load(Textures::Stone, "data/Editor/stone.png");
    mTextures.load(Textures::Barrier, "data/Editor/barrier.png");
    mTextures.load(Textures::Home, "data/Editor/home.png");
    mTextures.load(Textures::Castle, "data/Editor/castle.png");
    mTextures.load(Textures::RotateUp, "data/Editor/up.png");
    mTextures.load(Textures::RotateRight, "data/Editor/right.png");


    mFonts.load(Fonts::Main, "data/font.ttf");

    // Chargement des effets sonores
    mSounds.load(Sounds::Menu, "data/audio/click.wav");

    mStatisticsText.setFont(mFonts.get(Fonts::Main));
    mStatisticsText.setPosition(.5f, .5f);
    mStatisticsText.setCharacterSize(10u);

    // Initialisation de la statestack
    registerStates();
    mStateStack.pushState(States::MainMenu);
    mStateStack.handleEvent(sf::Event()); // Temp solution for initial startup
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
    //mStateStack.registerState<GameState>(States::Game);
}
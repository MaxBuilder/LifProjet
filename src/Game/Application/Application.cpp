#include "Application.hpp"

#include "../GameStates/MainMenuState.hpp"
#include "../GameStates/MapEditorState.hpp"
#include "../GameStates/SettingsState.hpp"
#include "../GameStates/GameState.hpp"
#include "../GameStates/PauseState.hpp"

const sf::Time Application::TimePerFrame = sf::seconds(1.f/60.f);
const sf::Time Accelerate = sf::seconds(1.f/12.f);

Application::Application()
: mTextures()
, mFonts()
, mSounds()
, mStateStack(State::Context(mWindow, mTextures, mFonts, mSounds))
, mStatisticsText()
, mStatisticsUpdateTime()
, mStatisticsNumFrames(0)
, mAccelerate(false)
{
    loadSettings();

    mWindow.create(mVideoMode,"LifProjet",mWindowStyle);
    mView.setSize(1280,720);
    mView.setCenter(1280/2.f, 720/2.f);
    mView.setViewport(sf::FloatRect(0, 0, 1, 1));

    // Chargement des ressources
    loadTextures();

    // Chargement des polices de texte
    mFonts.load(Fonts::Main, "data/font.ttf");

    // Chargement des effets sonores
    mSounds.load(Sounds::Menu, "data/audio/click.wav");

    mStatisticsText.setFont(mFonts.get(Fonts::Main));
    mStatisticsText.setPosition(.5f, .5f);
    mStatisticsText.setCharacterSize(10u);

    // Initialisation de la statestack
    registerStates();
    mStateStack.pushState(States::MainMenu);
    mStateStack.handleEvent(sf::Event()); // Pas mieux pour l'instant
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
            if (mAccelerate)
                update(Accelerate);
            else
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
        mWindow.setView(mView);
        mStateStack.handleEvent(event);

        if (event.type == sf::Event::Closed)
            mWindow.close();
        if(event.type == sf::Event::KeyPressed and event.key.code == sf::Keyboard::Space)
            mAccelerate = not mAccelerate;
        if(event.type ==sf::Event::MouseButtonPressed) {
            // sf::Vector2i mouse = sf::Mouse::getPosition(mWindow);
            // std::cout << mouse.x << " " << mouse.y<<std::endl;
        }
    }
}

void Application::update(sf::Time dt) {
    mStateStack.update(dt);
}

void Application::render() {
    mWindow.clear();

    mWindow.setView(mView);
    mStateStack.draw();

    // mWindow.setView(mWindow.getDefaultView());
    // mWindow.draw(mStatisticsText);

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

void Application::loadTextures() {
    mTextures.load(Textures::MenuBackground1, "data/background/background_menu1.png");
    mTextures.load(Textures::MenuBackground2, "data/background/background_menu2.png");
    mTextures.load(Textures::MenuBackground3, "data/background/background_menu3.png");
    mTextures.load(Textures::MenuBackground4, "data/background/background_menu4.png");
    mTextures.load(Textures::EditorBackground, "data/background/mapEditorBackGround.png");
    mTextures.load(Textures::MenuButton, "data/menuButton.png");
    mTextures.load(Textures::MenuTitle, "data/menuTitle.png");
    mTextures.load(Textures::Checkbox, "data/checkbox.png");
    mTextures.load(Textures::SettingsButton, "data/settingsButton.png");

    mTextures.load(Textures::MapGround, "data/texturesMap.png");

    mTextures.load(Textures::EditorBackButton, "data/Editor/back.png");
    mTextures.load(Textures::EditorNewButton, "data/Editor/new.png");
    mTextures.load(Textures::EditorLoadButton, "data/Editor/load.png");
    mTextures.load(Textures::EditorSaveButton, "data/Editor/save.png");
    mTextures.load(Textures::EditorUndoButton, "data/Editor/undo.png");
    mTextures.load(Textures::EditorRedoButton, "data/Editor/redo.png");
    mTextures.load(Textures::EditorEntityButton, "data/Editor/entityButton.png");
    mTextures.load(Textures::EditorTeamColor, "data/Editor/teamColor.png");
    mTextures.load(Textures::EditorSoldier, "data/Editor/soldier.png");
    mTextures.load(Textures::EditorArcher, "data/Editor/archer.png");
    mTextures.load(Textures::EditorTank, "data/Editor/tank.png");
    mTextures.load(Textures::EditorBuilding, "data/Editor/building.png");
    mTextures.load(Textures::EditorAllSoldiers, "data/Editor/allSoldiers.png");
    mTextures.load(Textures::ToolSmallBrush, "data/Editor/small.png");
    mTextures.load(Textures::ToolMediumBrush, "data/Editor/medium.png");
    mTextures.load(Textures::ToolBigBrush, "data/Editor/big.png");
    mTextures.load(Textures::ToolEraser, "data/Editor/Eraser.png");
    mTextures.load(Textures::ToolFill, "data/Editor/Fill.png");
    mTextures.load(Textures::EditorGrassButton, "data/Editor/grass.png");
    mTextures.load(Textures::EditorSandButton, "data/Editor/sand.png");
    mTextures.load(Textures::EditorWaterButton, "data/Editor/water.png");
    mTextures.load(Textures::EditorWoodButton, "data/Editor/wood.png");
    mTextures.load(Textures::EditorStoneButton, "data/Editor/stone.png");
    mTextures.load(Textures::EditorBarrierButton, "data/Editor/barrier.png");
    mTextures.load(Textures::EditorHomeButton, "data/Editor/home.png");
    mTextures.load(Textures::EditorCastleButton, "data/Editor/castle.png");
    mTextures.load(Textures::EditorRotateUpButton, "data/Editor/up.png");
    mTextures.load(Textures::EditorRotateRightButton, "data/Editor/right.png");
    mTextures.load(Textures::SubBackground, "data/Editor/subBackground.png");
    mTextures.load(Textures::EntitySoldier, "data/spritesSoldiers.png");
    mTextures.load(Textures::EntityGlow, "data/glow.png");
}

void Application::registerStates() {
    mStateStack.registerState<MainMenuState>(States::MainMenu);
    mStateStack.registerState<MapEditorState>(States::MapEditor);
    mStateStack.registerState<GameState>(States::Game);
    mStateStack.registerState<PauseState>(States::Pause);
    mStateStack.registerState<SettingsState>(States::Settings);


}

void Application::loadSettings() {
    std::ifstream settings("data/Settings/Settings.txt");

    if(!settings.is_open()) {
        std::cout << "Cannot open Settings file!" << std::endl;
        return ;
    }
    int height, width;
    std::string tmp;

    settings >> tmp;
    settings >> mWindowStyle;
    settings >> tmp;
    settings >> width;
    settings >> tmp;
    settings >> height;

    mVideoMode = sf::VideoMode(width,height);
    settings.close();
}
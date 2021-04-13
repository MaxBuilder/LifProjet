#include "Application.hpp"

#include "../GameStates/MainMenuState.hpp"
#include "../GameStates/MapEditorState.hpp"
#include "../GameStates/SettingsState.hpp"
#include "../GameStates/GameState.hpp"
#include "../GameStates/PauseState.hpp"

const sf::Time Application::TimePerFrame = sf::seconds(1.f/60.f);

Application::Application()
: mTextures()
, mFonts()
, mSounds()
, mMusic()
, mStateStack(State::Context(mWindow, mTextures, mFonts, mSounds, mMusic))
{
    loadSettings();

    mWindow.create(mVideoMode,"LifProjet", mWindowStyle);
    mView.setSize(1280,720);
    mView.setCenter(1280 / 2.f, 720 / 2.f);
    mView.setViewport(sf::FloatRect(0, 0, 1, 1));

    // Chargement des textures
    loadTextures();

    // Chargement des polices de texte
    mFonts.load(Fonts::Main, "data/Fonts/font.ttf");

    // Chargement des effets sonores
    mSounds.load(Sounds::Menu, "data/Audio/click.wav");

    // Initialisation de la statestack
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

            if (mStateStack.isEmpty())
                mWindow.close();
        }

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
    }
}

void Application::update(sf::Time dt) {
    mStateStack.update(dt);
}

void Application::render() {
    mWindow.clear();

    mWindow.setView(mView);
    mStateStack.draw();

    mWindow.display();
}

void Application::loadTextures() {
    mTextures.load(Textures::MenuBackground1, "data/Menu/background_menu1.png");
    mTextures.load(Textures::MenuBackground2, "data/Menu/background_menu2.png");
    mTextures.load(Textures::MenuBackground3, "data/Menu/background_menu3.png");
    mTextures.load(Textures::MenuBackground4, "data/Menu/background_menu4.png");
    mTextures.load(Textures::MenuButton, "data/Menu/menuButton.png");
    mTextures.load(Textures::MenuTitle, "data/Menu/menuTitle.png");
    mTextures.load(Textures::SettingsButton, "data/Menu/settingsButton.png");

    mTextures.load(Textures::Checkbox, "data/Settings/checkbox.png");
    mTextures.load(Textures::SettingsBack, "data/Settings/back.png");
    mTextures.load(Textures::SettingsBackground, "data/Settings/background.png");
    mTextures.load(Textures::SettingsSave, "data/Settings/save.png");
    mTextures.load(Textures::SettingsApply, "data/Settings/apply.png");

    mTextures.load(Textures::MapGround, "data/Tiles/texturesMap.png");

    mTextures.load(Textures::EditorBackground, "data/Editor/mapEditorBackGround.png");
    mTextures.load(Textures::SubBackground, "data/Editor/subBackground.png");
    mTextures.load(Textures::EditorBackButton, "data/Editor/back.png");
    mTextures.load(Textures::EditorNewButton, "data/Editor/new.png");
    mTextures.load(Textures::EditorLoadButton, "data/Editor/load.png");
    mTextures.load(Textures::EditorSaveButton, "data/Editor/save.png");
    mTextures.load(Textures::EditorUndoButton, "data/Editor/undo.png");
    mTextures.load(Textures::EditorRedoButton, "data/Editor/redo.png");
    mTextures.load(Textures::EditorTeamColor, "data/Editor/teamColor.png");
    mTextures.load(Textures::EditorKnight, "data/Editor/soldier.png");
    mTextures.load(Textures::EditorArcher, "data/Editor/archer.png");
    mTextures.load(Textures::EditorTank, "data/Editor/tank.png");
    mTextures.load(Textures::EditorBuilding, "data/Editor/building.png");
    mTextures.load(Textures::EditorAllSoldiers, "data/Editor/icons.png");
    mTextures.load(Textures::ToolSmallBrush, "data/Editor/small.png");
    mTextures.load(Textures::ToolMediumBrush, "data/Editor/medium.png");
    mTextures.load(Textures::ToolBigBrush, "data/Editor/big.png");
    mTextures.load(Textures::ToolEraser, "data/Editor/Eraser.png");
    mTextures.load(Textures::ToolFill, "data/Editor/Fill.png");
    mTextures.load(Textures::EditorRotateUpButton, "data/Editor/up.png");
    mTextures.load(Textures::EditorRotateRightButton, "data/Editor/right.png");

    mTextures.load(Textures::EntityKnightBlue, "data/Entities/spriteKnightBlue.png");
    mTextures.load(Textures::EntityKnightRed, "data/Entities/spriteKnightRed.png");
    mTextures.load(Textures::EntityArcherBlue, "data/Entities/spriteArcherBlue.png");
    mTextures.load(Textures::EntityArcherRed, "data/Entities/spriteArcherRed.png");
    mTextures.load(Textures::EntityTankRed, "data/Entities/spriteTankRed.png");
    mTextures.load(Textures::EntityTankBlue, "data/Entities/spriteTankBlue.png");
    mTextures.load(Textures::EntityGlow, "data/Entities/glow.png");
    mTextures.load(Textures::EntityArrow, "data/Entities/arrow.png");

    mTextures.load(Textures::GamePause, "data/Game/pause.png");
    mTextures.load(Textures::GameSlow, "data/Game/slowMode.png");
    mTextures.load(Textures::Game1x, "data/Game/1xspeed.png");
    mTextures.load(Textures::Game3x, "data/Game/3xspeed.png");
    mTextures.load(Textures::Game5x, "data/Game/5xspeed.png");
    mTextures.load(Textures::GameTimeUI, "data/Game/Time.png");
    mTextures.load(Textures::GameMapSelectionButton, "data/Game/mapSelectButton.png");
    mTextures.load(Textures::GameMapSelectionConfButton, "data/Game/mapConfButton.png");
    mTextures.load(Textures::GameMapSelectionBackground, "data/Game/mapSelectionBackground.png");
    mTextures.load(Textures::GameVictoryScreen, "data/Game/victoryScreen.png");
}

void Application::registerStates() {
    mStateStack.registerState<MainMenuState>(States::MainMenu);
    mStateStack.registerState<MapEditorState>(States::MapEditor);
    mStateStack.registerState<GameState>(States::Game);
    mStateStack.registerState<PauseState>(States::Pause);
    mStateStack.registerState<SettingsState>(States::Settings);
}

void Application::loadSettings() {
    std::ifstream settings("data/Settings/settings.txt");

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

    mVideoMode = sf::VideoMode(width, height);
    settings.close();
}
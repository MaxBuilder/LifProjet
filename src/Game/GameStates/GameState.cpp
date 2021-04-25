//
// Created by 33771 on 14/02/2021.
//

#include "GameState.hpp"

GameState::GameState(StateStack &stack, Context& context)
: State(stack, context)
, mView()
, mWorld(context.window, context.textures, context.fonts, context.sounds, context.music)
, mDirection(0, 0)
, mScroll(0)
, mSpeed(0)
, mFactor(1.f)
, mTracking(false)
, mTimeSpeed(1)
, mTrackText("Tracking Soldier", context.fonts.get(Fonts::Main))
, mTimeText("", context.fonts.get(Fonts::Main))
, mTime()
, mTimeUI(context.textures.get(Textures::GameTimeUI))
, isLoaded(false)
, mMapSelectBackground(context.textures.get(Textures::GameMapSelectionBackground))
, mMapSelectText("Choose a scenario : ", context.fonts.get(Fonts::Main))
, mVictoryText("Victory", context.fonts.get(Fonts::Main))
, mVictoryScreen(context.textures.get(Textures::GameVictoryScreen))
, ended(false)
, mVictoryInfo("", context.fonts.get(Fonts::Main))
, closed(false)
, simData(mWorld.getSimData())
{
    mView.setSize(1280, 720);
    mView.setCenter(640, 360);

    // Initialisation des affichages :

    mTrackText.setPosition(2, 2);
    mTrackText.setCharacterSize(25u);

    mTimeText.setCharacterSize(26u);
    mTimeText.setPosition(538, 4);
    mTimeText.setFillColor(sf::Color::Black);

    mTimeUI.setPosition(520, 0);

    mMapSelectBackground.setPosition(374, 97);
    centerOrigin(mMapSelectText);
    mMapSelectText.setPosition(640, 129);

    mVictoryText.setCharacterSize(100u);
    mVictoryText.setStyle(sf::Text::Bold);
    centerOrigin(mVictoryText);
    mVictoryText.setPosition(640, 220);
    mVictoryScreen.setPosition(445, 150);
    mVictoryInfo.setCharacterSize(22u);
    mVictoryInfo.setFillColor(sf::Color::Black);

    // Barres d'état des équipes :

    mRedDisplay.setFillColor(sf::Color::Red);
    mRedDisplay.setPosition(sf::Vector2f(622, 9));
    mBlueDisplay.setFillColor(sf::Color::Blue);

    // Construction de l'UI :

    auto pauseButton = std::make_shared<GUI::Button>(context, 45, 45, Textures::GamePause);
    pauseButton->setPosition(1055, 0);
    pauseButton->setToggle(true);
    pauseButton->setCallback([this] () {
        mTimeSpeed = 0;
        getContext().sounds.play(Sounds::Menu);
    });
    mUI.pack(pauseButton);

    auto slowButton = std::make_shared<GUI::Button>(context, 45, 45, Textures::GameSlow);
    slowButton->setPosition(1100, 0);
            slowButton->setToggle(true);
    slowButton->setCallback([this] () {
        mTimeSpeed = 0.5;
        getContext().sounds.play(Sounds::Menu);
    });
    mUI.pack(slowButton);

    auto x1Button = std::make_shared<GUI::Button>(context, 45, 45, Textures::Game1x);
    x1Button->setPosition(1145, 0);
    x1Button->setToggle(true);
    x1Button->setCallback([this] () {
        mTimeSpeed = 1;
        getContext().sounds.play(Sounds::Menu);
    });
    x1Button->activate();
    mUI.pack(x1Button);

    auto x3Button = std::make_shared<GUI::Button>(context, 45, 45, Textures::Game3x);
    x3Button->setPosition(1190, 0);
    x3Button->setToggle(true);
    x3Button->setCallback([this] () {
        mTimeSpeed = 3;
        getContext().sounds.play(Sounds::Menu);
    });
    mUI.pack(x3Button);

    auto x5Button = std::make_shared<GUI::Button>(context, 45, 45, Textures::Game5x);
    x5Button->setPosition(1235, 0);
    x5Button->setToggle(true);
    x5Button->setCallback([this] () {
        mTimeSpeed = 5;
        getContext().sounds.play(Sounds::Menu);
    });
    mUI.pack(x5Button);

    // Construction de l'UI de selection de scénario :

    std::unordered_map<std::string, std::string> mapPath = {{"Demo complete (temp)", "scenario7"}, {"Demo (temp)", "scenario6"}, {"Target priority", "scenario5"},
                                                            {"Buildings destruction", "scenario4"}, {"Group combat", "scenario3"}, {"1v1", "scenario2"}, {"Pathfinding", "scenario1"},
                                                            {"Custom map 3", "editor3"}, {"Custom map 2", "editor2"}, {"Custom map 1", "editor1"}};

    int i = 0;
    for(const auto& [name, path] : mapPath) {
        auto temp = std::make_shared<GUI::Button>(getContext(), 500, 40, Textures::GameMapSelectionButton);
        temp->setText(name);
        temp->setToggle(true);
        temp->setPosition(390, (float)(155 + i * 40));
        temp->setCallback([=, path = path] () {
            getContext().sounds.play(Sounds::Menu);
            mMapPath = "data/MapData/" + path + ".map";
        });
        if(i == 0) temp->activate();
        i++;

        mMapSelectionUI.pack(temp);
    }

    auto confirmButton = std::make_shared<GUI::Button>(getContext(), 170, 40, Textures::GameMapSelectionConfButton);
    confirmButton->setPosition(470, 567);
    confirmButton->setText("Confirm");
    confirmButton->setCallback([this] () {
        initializeSimulation();
        getContext().sounds.play(Sounds::Menu);
        getContext().music.play(Music::BattleTheme);
    });
    mMapSelectionUI.pack(confirmButton);

    auto cancelButton  = std::make_shared<GUI::Button>(getContext(), 170, 40, Textures::GameMapSelectionConfButton);
    cancelButton->setPosition(640, 567);
    cancelButton->setText("Back");
    cancelButton->setCallback([this] () {
        requestStackPop();
        requestStackPush(States::MainMenu);
        getContext().sounds.play(Sounds::Menu);
    });
    mMapSelectionUI.pack(cancelButton);

    // Interface après victoire :

    auto returnButton = std::make_shared<GUI::Button>(getContext(), 60, 60, Textures::EditorBackButton);
    returnButton->setPosition(20, 640);
    returnButton->setCallback([this] () {
        requestStackClear();
        requestStackPush(States::MainMenu);
        getContext().sounds.play(Sounds::Menu);
    });
    mBackButton.pack(returnButton);

    auto closeButton = std::make_shared<GUI::Button>(getContext(), 120, 50, Textures::GameVictoryCloseButton);
    closeButton->setPosition(580, 500);
    closeButton->setText("Close");
    closeButton->setCallback([this] () {
        closed = true;
        getContext().sounds.play(Sounds::Menu);
    });
    mVictoryUI.pack(closeButton);
}

void GameState::initializeSimulation() {
    mWorld.init(mMapPath);
    isLoaded = true;
}

void GameState::draw() {
    sf::RenderWindow& window = getContext().window;
    auto tempView = window.getView();

    window.setView(mView);

    mWorld.draw();

    if(mTracking) {
        window.setView(window.getDefaultView());
        window.draw(mTrackText);
    }

    window.setView(tempView);
    if(ended) {
        if(!closed) {
            window.draw(mVictoryScreen);
            window.draw(mVictoryText);

            mVictoryInfo.setPosition(475, 275);
            for (const auto &info : mVictoryInfoTab) {
                mVictoryInfo.setString(info);
                window.draw(mVictoryInfo);
                mVictoryInfo.move(0, (float) mVictoryInfo.getCharacterSize());
            }

            window.draw(mVictoryUI);
        }

        window.draw(mBackButton);
        return;
    }

    if(isLoaded) {
        window.draw(mTimeUI);
        window.draw(mTimeText);
        window.draw(mRedDisplay);
        window.draw(mBlueDisplay);
        window.draw(mUI);
    }
    else {
        window.draw(mMapSelectBackground);
        window.draw(mMapSelectText);
        window.draw(mMapSelectionUI);
    }
}

bool GameState::update(sf::Time dt) {
    if (simData.isEnded() and !ended) {
        mTimeText.setString("Ended");
        simData.mRedVictory ? mVictoryText.setFillColor(sf::Color::Red) : mVictoryText.setFillColor(sf::Color::Blue);
        mWorld.end();
        mTimeSpeed = 1;
        ended = true;

        float soldierRedPerc = float(simData.nbRedSoldierBegin - simData.nbRedSoldierEnd) / simData.nbRedSoldierBegin * 100.f;
        float soldierBluePerc = float(simData.nbBlueSoldierBegin - simData.nbBlueSoldierEnd) / simData.nbBlueSoldierBegin * 100.f;
        //int buildRedPerc = simData.nbRedBuildingBegin > 0 ?  (simData.nbRedBuildingBegin - simData.nbRedBuildingEnd) / simData.nbRedBuildingBegin * 100 : 0;
        //int buildBluePerc = simData.nbBlueBuildingBegin > 0 ? (simData.nbBlueBuildingBegin - simData.nbBlueBuildingEnd) / simData.nbBlueBuildingBegin * 100 : 0;
        mVictoryInfoTab.emplace_back("Red team : ");
        if (simData.nbBlueSoldierBegin > 0)
            mVictoryInfoTab.emplace_back("\t- Soldier killed : " + std::to_string(simData.nbBlueSoldierBegin - simData.nbBlueSoldierEnd) + " (" + std::to_string((int)soldierBluePerc) + "%)");
        else
            mVictoryInfoTab.emplace_back("\t- Soldier killed : " + std::to_string(simData.nbBlueSoldierBegin - simData.nbBlueSoldierEnd));

        if(simData.nbRedSoldierBegin > 0)
            mVictoryInfoTab.emplace_back("\t- Casualties : " + std::to_string(simData.nbRedSoldierBegin - simData.nbRedSoldierEnd) + " (" + std::to_string((int)soldierRedPerc) + "%)");
        else
            mVictoryInfoTab.emplace_back("\t- Casualties : " + std::to_string(simData.nbRedSoldierBegin - simData.nbRedSoldierEnd));
        mVictoryInfoTab.emplace_back("\t- Buildings destroyed : " + std::to_string(simData.nbBlueBuildingBegin - simData.nbBlueBuildingEnd));
        mVictoryInfoTab.emplace_back("");
        mVictoryInfoTab.emplace_back("Blue team : ");
        if(simData.nbRedSoldierBegin > 0)
            mVictoryInfoTab.emplace_back("\t- Soldier killed : " + std::to_string(simData.nbRedSoldierBegin - simData.nbRedSoldierEnd) + " (" + std::to_string((int)soldierRedPerc) + "%)");
        else
            mVictoryInfoTab.emplace_back("\t- Soldier killed : " + std::to_string(simData.nbRedSoldierBegin - simData.nbRedSoldierEnd));
        if (simData.nbBlueSoldierBegin > 0)
            mVictoryInfoTab.emplace_back("\t- Casualties : " + std::to_string(simData.nbBlueSoldierBegin - simData.nbBlueSoldierEnd) + " (" + std::to_string((int)soldierBluePerc) + "%)");
        else
            mVictoryInfoTab.emplace_back("\t- Casualties : " + std::to_string(simData.nbBlueSoldierBegin - simData.nbBlueSoldierEnd));
        mVictoryInfoTab.emplace_back("\t- Buildings destroyed : " + std::to_string(simData.nbRedBuildingBegin - simData.nbRedBuildingEnd));

        return false;
    }

    if(!mTracking) {
        mView.move(mDirection.x * mSpeed * dt.asSeconds() * mFactor, mDirection.y * mSpeed * dt.asSeconds() * mFactor );

        if (mScroll < 0) {
            mView.zoom(1.5f);
            mFactor *= 1.5f;
        }
        if (mScroll > 0) {
            mView.zoom(1.f/1.5f);
            mFactor *= (1.f/1.5f);
        }
        mScroll = 0;
    }
    else {
        mView.setCenter(mWorld.trackedPos());
        mView.setSize(640, 360);
    }

    dt = dt * mTimeSpeed;
    mWorld.update(dt);

    if(ended)
        return false;

    mTime += dt;
    int min = (int)mTime.asSeconds() / 60;
    int sec = (int)(mTime.asSeconds() - (float)min * 60);
    mTimeText.setString("0" + std::to_string(min) + ":" + (sec < 10 ? "0" : "") + std::to_string(sec));
    float ratio = (float)simData.nbRedSoldierEnd / (float)(simData.nbRedSoldierEnd + simData.nbBlueSoldierEnd);
    mRedDisplay.setSize(sf::Vector2f(ratio * 120, 24));
    mBlueDisplay.setPosition(mRedDisplay.getPosition().x + ratio * 120, 9);
    mBlueDisplay.setSize(sf::Vector2f(120 - mRedDisplay.getSize().x, 24));

    return false;
}

bool GameState::handleEvent(const sf::Event &event) {
    if(!isLoaded) {
        mMapSelectionUI.handleEvent(event, getContext().window);
        return false;
    }
    else if(ended) {
        if(!closed)
            mVictoryUI.handleEvent(event, getContext().window);
        mBackButton.handleEvent(event, getContext().window);
    }
    else mUI.handleEvent(event, getContext().window);

    mDirection = sf::Vector2f(0.f, 0.f);
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Z)) mDirection += sf::Vector2f(0.f, -1.f);
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::S)) mDirection += sf::Vector2f(0.f, +1.f);
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Q)) mDirection += sf::Vector2f(-1.f, 0.f);
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::D)) mDirection += sf::Vector2f(1.f, 0.f);

    float norme = norm(mDirection);
    if (norme > 0) mDirection = mDirection / norme;

    if(!mTracking) {
        mSpeed = sf::Keyboard::isKeyPressed(sf::Keyboard::LShift) ? 1500 : 800;

        if( event.type == sf::Event::MouseWheelScrolled)
            mScroll = (int) event.mouseWheelScroll.delta;
        else
            mScroll = 0;
    }

    if(event.type == sf::Event::KeyPressed and event.key.code == sf::Keyboard::E) {
        mWorld.switchDisplayDebug();
    }

    if(sf::Keyboard::isKeyPressed(sf::Keyboard::Escape))
        requestStackPush(States::Pause);

    // Tracking
    if(sf::Keyboard::isKeyPressed(sf::Keyboard::Left)) {
        if(!mTracking)
            mTracking = true;
        mWorld.trackPrev();
    }
    else if(sf::Keyboard::isKeyPressed(sf::Keyboard::Right)) {
        if(!mTracking)
            mTracking = true;
        mWorld.trackNext();
    }

    if(sf::Keyboard::isKeyPressed(sf::Keyboard::F)) {
        if(mTracking) {
            mWorld.untrack();
        }
        mTracking = false;
    }

    return false;
}

//
// Created by 33771 on 14/02/2021.
//

#include "GameState.hpp"

GameState::GameState(StateStack &stack, Context& context)
: State(stack, context)
, mView()
, mWorld(context.window, context.textures, context.fonts, context.sounds)
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
{
    mView.setSize(1280, 720);
    mView.setCenter(640, 360);

    // Initialisation des affichages :

    mTrackText.setPosition(2, 2);
    mTrackText.setCharacterSize(25u);

    mTimeText.setCharacterSize(16u);
    mTimeText.setPosition(571, 4);
    mTimeText.setFillColor(sf::Color::Black);

    mTimeUI.setPosition(560, 0);

    mMapSelectBackground.setPosition(374, 97);
    centerOrigin(mMapSelectText);
    mMapSelectText.setPosition(640, 129);

    mVictoryText.setCharacterSize(100u);
    mVictoryText.setStyle(sf::Text::Bold);
    centerOrigin(mVictoryText);
    mVictoryText.setPosition(640, 300);
    mVictoryScreen.setPosition(340, 220);

    // Barres d'état des équipes :

    mRedDisplay.setFillColor(sf::Color::Red);
    mRedDisplay.setPosition(sf::Vector2f(627, 6));
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

    std::vector<std::string> mapPath = {"demo1", "demo2", "demo3", "demo1", "demo1", "demo1", "demo1", "demo1", "demo1", "demo1"};
    for(int i = 0 ; i < mapPath.size() ; i++) {
        auto temp = std::make_shared<GUI::Button>(getContext(), 500, 40, Textures::GameMapSelectionButton);
        temp->setText(mapPath.at(i));
        temp->setToggle(true);
        temp->setPosition(390, 155 + i * 40);
        temp->setCallback([=] () {
            getContext().sounds.play(Sounds::Menu);
            mMapPath = "data/MapData/" + mapPath.at(i) + ".map";
        });
        if(i == 0) temp->activate();

        mMapSelectionUI.pack(temp);
    }

    auto confirmButton = std::make_shared<GUI::Button>(getContext(), 170, 40, Textures::GameMapSelectionConfButton);
    confirmButton->setPosition(470, 567);
    confirmButton->setText("Confirm");
    confirmButton->setCallback([this] () {
        getContext().sounds.play(Sounds::Menu);
        initializeSimulation();
    });
    mMapSelectionUI.pack(confirmButton);

    auto cancelButton  = std::make_shared<GUI::Button>(getContext(), 170, 40, Textures::GameMapSelectionConfButton);
    cancelButton->setPosition(640, 567);
    cancelButton->setText("Back");
    cancelButton->setCallback([this] () {
        requestStackClear();
        requestStackPush(States::MainMenu);
        getContext().sounds.play(Sounds::Menu);
    });
    mMapSelectionUI.pack(cancelButton);

    // Bouton retour lors de la fin de la simulation :

    auto returnButton = std::make_shared<GUI::Button>(getContext(), 60, 60, Textures::EditorBackButton);
    returnButton->setPosition(20, 640);
    returnButton->setCallback([this] () {
        requestStackClear();
        requestStackPush(States::MainMenu);
        getContext().sounds.play(Sounds::Menu);
    });
    mBack.pack(returnButton);
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
        window.draw(mVictoryScreen);
        window.draw(mVictoryText);
        window.draw(mBack);
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
    if (mWorld.isEnded()){
        mTimeText.setString("Ended");
        mWorld.returnVictoryState() ? mVictoryText.setFillColor(sf::Color::Red) : mVictoryText.setFillColor(sf::Color::Blue);
        ended = true;
        return true;
    }

    if(!mTracking) {
        mView.move(mDirection.x * mSpeed *dt.asSeconds() * mFactor, mDirection.y * mSpeed *dt.asSeconds() * mFactor );

        if (mScroll < 0){
            mView.zoom(1.5f);
            mFactor *= 1.5f;
        }
        if (mScroll > 0){
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

    mTime += dt;
    int min = (int)mTime.asSeconds() / 60;
    int sec = (int)(mTime.asSeconds() - (float)min * 60);
    mTimeText.setString("0" + std::to_string(min) + " : " + (sec < 10 ? "0" : "") + std::to_string(sec));
    auto rem = mWorld.getRemaining();
    float ratio = (float)rem.first / (float)(rem.first + rem.second);
    mRedDisplay.setSize(sf::Vector2f(ratio * 80, 16));
    mBlueDisplay.setPosition(mRedDisplay.getPosition().x + ratio * 80, 6);
    mBlueDisplay.setSize(sf::Vector2f(80 - mRedDisplay.getSize().x, 16));

    return true;
}

bool GameState::handleEvent(const sf::Event &event) {
    if(!isLoaded) {
        mMapSelectionUI.handleEvent(event, getContext().window);
        return false;
    }
    else if(ended) {
        mBack.handleEvent(event, getContext().window);
        return false;
    }

    mDirection = sf::Vector2f(0.f, 0.f);
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Z)) mDirection += sf::Vector2f(0.f, -1.f);
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::S)) mDirection += sf::Vector2f(0.f, +1.f);
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Q)) mDirection += sf::Vector2f(-1.f, 0.f);
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::D)) mDirection += sf::Vector2f(1.f, 0.f);

    float norme = norm(mDirection);
    if (norme > 0) mDirection = mDirection / norme;

    if(!mTracking) {
        mSpeed = sf::Keyboard::isKeyPressed(sf::Keyboard::LShift) ? 1500 : 800;

        if( event.type == sf::Event::MouseWheelMoved)
            mScroll = (int) event.mouseWheelScroll.delta;
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

    mUI.handleEvent(event, getContext().window);

    return false;
}

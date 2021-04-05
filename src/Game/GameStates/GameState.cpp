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
, mTracking(false)
, mTimeSpeed(1)
, mTrackText("Tracking Soldier", context.fonts.get(Fonts::Main))
, mTimeText("", context.fonts.get(Fonts::Main))
, mTime()
, mTimeUI(context.textures.get(Textures::GameTimeUI))
{
    mView.setSize(1280, 720);
    mView.setCenter(640, 360);

    mTrackText.setPosition(2, 2);
    mTrackText.setCharacterSize(25u);

    mTimeText.setCharacterSize(16u);
    mTimeText.setPosition(571, 4);
    mTimeText.setFillColor(sf::Color::Black);

    mTimeUI.setPosition(560, 0);

    // Barres d'état des équipes :
    mRedDisplay.setFillColor(sf::Color::Red);
    mRedDisplay.setPosition(sf::Vector2f(627, 6));
    mBlueDisplay.setFillColor(sf::Color::Blue);

    // Construction de l'UI :
    auto pauseButton = std::make_shared<GUI::Button>(context, 40, 40, Textures::GamePause);
    pauseButton->setPosition(1120, 0);
    pauseButton->setToggle(true);
    pauseButton->setCallback([this] () {
        mTimeSpeed = 0;
        getContext().sounds.play(Sounds::Menu);
    });
    mUI.pack(pauseButton);

    auto x1Button = std::make_shared<GUI::Button>(context, 40, 40, Textures::Game1x);
    x1Button->setPosition(1160, 0);
    x1Button->setToggle(true);
    x1Button->setCallback([this] () {
        mTimeSpeed = 1;
        getContext().sounds.play(Sounds::Menu);
    });
    x1Button->activate();
    mUI.pack(x1Button);

    auto x3Button = std::make_shared<GUI::Button>(context, 40, 40, Textures::Game3x);
    x3Button->setPosition(1200, 0);
    x3Button->setToggle(true);
    x3Button->setCallback([this] () {
        mTimeSpeed = 3;
        getContext().sounds.play(Sounds::Menu);
    });
    mUI.pack(x3Button);

    auto x5Button = std::make_shared<GUI::Button>(context, 40, 40, Textures::Game5x);
    x5Button->setPosition(1240, 0);
    x5Button->setToggle(true);
    x5Button->setCallback([this] () {
        mTimeSpeed = 5;
        getContext().sounds.play(Sounds::Menu);
    });
    mUI.pack(x5Button);

}

void GameState::draw() {
    sf::RenderWindow& window = getContext().window;

    window.setView(mView);

    mWorld.draw();

    if(mTracking) {
        window.setView(window.getDefaultView());
        window.draw(mTrackText);
    }

    window.setView(window.getDefaultView());
    window.draw(mTimeUI);
    window.draw(mTimeText);
    window.draw(mRedDisplay);
    window.draw(mBlueDisplay);
    window.draw(mUI);
}

bool GameState::update(sf::Time dt) {
    if(!mTracking) {
        mView.move(mDirection.x * mSpeed * dt.asSeconds(), mDirection.y * mSpeed * dt.asSeconds());

        if (mScroll < 0) mView.zoom(2.f);
        if (mScroll > 0) mView.zoom(0.5f);
        mScroll = 0;
    }
    else {
        mView.setCenter(mWorld.trackedPos());
        mView.setSize(640, 360);
    }

    dt = dt * (float)mTimeSpeed;
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
    mDirection = sf::Vector2f(0.f, 0.f);
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Z)) mDirection += sf::Vector2f(0.f, -1.f);
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::S)) mDirection += sf::Vector2f(0.f, +1.f);
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Q)) mDirection += sf::Vector2f(-1.f, 0.f);
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::D)) mDirection += sf::Vector2f(1.f, 0.f);

    float norme = norm(mDirection);
    if (norme > 0) mDirection = mDirection / norme;

    if(!mTracking) {
        mSpeed = sf::Keyboard::isKeyPressed(sf::Keyboard::LShift) ? 1500 : 800;

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
//
// Created by 33771 on 14/02/2021.
//

#include "GameState.hpp"
#include <cmath>
#include <iostream>

GameState::GameState(StateStack &stack, Context& context)
: State(stack, context)
, mView()
//, mBackground(context.textures.get(Textures::None))
, mWorld(context.window, context.textures, context.fonts, context.sounds)
, mDirection(0, 0)
, mScroll(0)
, mSpeed(0)
, mTracking(false)
, mTrackText("Tracking Soldier", context.fonts.get(Fonts::Main))
{
    mView.setSize(1280, 720);
    mView.setCenter(640, 360);

    mTrackText.setPosition(2, 2);
    mTrackText.setCharacterSize(25u);
}

void GameState::draw() {
    sf::RenderWindow& window = getContext().window;

    window.setView(mView);

    mWorld.draw();

    if(mTracking) {
        window.setView(window.getDefaultView());
        window.draw(mTrackText);
    }
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
        mWorld.trackedMove(mDirection);
    }

    mWorld.update(dt);

    return true;
}

bool GameState::handleEvent(const sf::Event &event) {
    // Temp to move entity
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
            mWorld.trackedReset();
            mWorld.untrack();
        }
        mTracking = false;
    }

    return false;
}
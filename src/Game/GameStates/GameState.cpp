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
{
    //mView.setViewport(sf::FloatRect(0, 0, 1, 1));
    mView.setSize(1280, 720);
    mView.setCenter(640, 360);
    //context.window.setView(mView);
}

void GameState::draw() {
    sf::RenderWindow& window = getContext().window;

    window.setView(mView);

    mWorld.draw();
    //window.draw(mBackground);
}

bool GameState::update(sf::Time dt) {
    mView.move(mDirection.x * mSpeed * dt.asSeconds(), mDirection.y * mSpeed * dt.asSeconds());

    if(mScroll < 0) mView.zoom(2.f);
    if(mScroll > 0) mView.zoom(0.5f);
    mScroll = 0;

    return true;
}

bool GameState::handleEvent(const sf::Event &event) {
    mDirection = sf::Vector2f(0.f, 0.f);
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Z)) mDirection += sf::Vector2f(0.f, -1.f);
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::S)) mDirection += sf::Vector2f(0.f, +1.f);
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Q)) mDirection += sf::Vector2f(-1.f, 0.f);
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::D)) mDirection += sf::Vector2f(1.f, 0.f);

    float norme = std::sqrt(mDirection.x * mDirection.x + mDirection.y * mDirection.y);
    if(norme > 0) mDirection = mDirection / norme;
    mSpeed = sf::Keyboard::isKeyPressed(sf::Keyboard::LShift) ? 1500 : 800;

    mScroll = (int)event.mouseWheelScroll.delta;

    if(sf::Keyboard::isKeyPressed(sf::Keyboard::Escape))
        requestStackPush(States::Pause);

    return false;
}
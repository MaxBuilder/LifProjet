//
// Created by 33771 on 14/02/2021.
//

#include "GameState.hpp"

GameState::GameState(StateStack &stack, Context& context)
: State(stack, context) {
}

void GameState::draw() {

}

bool GameState::update(sf::Time dt) {
    return true;
}

bool GameState::handleEvent(const sf::Event &event) {
    return false;
}
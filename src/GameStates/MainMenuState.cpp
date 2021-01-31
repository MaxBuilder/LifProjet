//
// Created by 33771 on 31/01/2021.
//

#include "MainMenuState.hpp"
#include "../Resources/ResourceHolder.hpp"

MainMenuState::MainMenuState(StateStack &stack, Context context)
: State(stack, context) {
    sf::Texture& texture = context.textures.get(Textures::Background);
    background.setTexture(texture);
}

void MainMenuState::draw() {
    sf::RenderWindow& window = getContext().window;
    window.draw(background);
}

bool MainMenuState::update(sf::Time dt) {
    return true;
}

bool MainMenuState::handleEvent(const sf::Event &event) {
    return true;
}
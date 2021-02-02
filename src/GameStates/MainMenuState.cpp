//
// Created by 33771 on 31/01/2021.
//

#include "MainMenuState.hpp"
#include "../Resources/ResourceHolder.hpp"
#include <iostream>

MainMenuState::MainMenuState(StateStack &stack, Context context)
: State(stack, context)
, mGUIContainer() {
    sf::Texture& texture = context.textures.get(Textures::Background);
    background.setTexture(texture);

    auto button = std::make_shared<GUI::Button>(context, 200, 50, Textures::Button);
    button->setPosition(100, 100);
    button->setText("Coucou");

    mGUIContainer.pack(button);
}

void MainMenuState::draw() {
    sf::RenderWindow& window = getContext().window;
    window.draw(background);
    window.draw(mGUIContainer);
}

bool MainMenuState::update(sf::Time dt) {
    return true;
}

bool MainMenuState::handleEvent(const sf::Event &event) {
    //std::cout << event << std::endl;
    mGUIContainer.handleEvent(event);
    return false;
}
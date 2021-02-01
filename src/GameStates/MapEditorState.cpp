//
// Created by thibaut on 01/02/2021.
//

#include "MapEditorState.hpp"
#include "../Resources/ResourceHolder.hpp"

MapEditorState::MapEditorState(StateStack &stack, Context context)
        : State(stack, context) {
    sf::Texture& texture = context.textures.get(Textures::Background);
    background.setTexture(texture);
}

void MapEditorState::draw() {
    sf::RenderWindow& window = getContext().window;
    window.draw(background);
}

bool MapEditorState::update(sf::Time dt) {
    return true;
}

bool MapEditorState::handleEvent(const sf::Event &event) {
    return true;
}
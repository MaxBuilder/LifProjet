//
// Created by thibaut on 01/02/2021.
//

#include "MapEditorState.hpp"
#include "../Resources/ResourceHolder.hpp"
#include <iostream>

MapEditorState::MapEditorState(StateStack &stack, Context context)
        : State(stack, context)
        , map(mapMode::mapEditor, context.textures.get(Textures::Map)){
}

void MapEditorState::draw() {
    sf::RenderWindow& window = getContext().window;
    window.draw(map);
}

bool MapEditorState::update(sf::Time dt) {
    return true;
}

bool MapEditorState::handleEvent(const sf::Event& event) {

    if(event.type == sf::Event::KeyPressed){
        std::cout<<"Keypressed\n";
        switchGround(event);
    }

    if (event.type == sf::Event::MouseMoved){

        sf::Vector2i position = sf::Mouse::getPosition(getContext().window);

        if (position.x < 1920*0.8f and position.x >= 0 and position.y >= 108 and position.y < 1080-108 ) // rectangle contenant la map

            map.handleEvent(event, getContext().window);
    }
    return false;
}

void MapEditorState::switchGround(const sf::Event& event){
    switch (event.key.code){

        case sf::Keyboard::Numpad0 : map.setGroundSelection(Textures::ground::Grass); break;

        case sf::Keyboard::Numpad1 : map.setGroundSelection(Textures::ground::Sand); break;

        case sf::Keyboard::Numpad2 : map.setGroundSelection(Textures::ground::Wood); break;

        case sf::Keyboard::Numpad3 : map.setGroundSelection(Textures::ground::Water); break;

        case sf::Keyboard::Numpad4 : map.setGroundSelection(Textures::ground::Wall); break;

        default : std::cout<<"default way : "<<event.key.code<<"\n"; break;

    }
}
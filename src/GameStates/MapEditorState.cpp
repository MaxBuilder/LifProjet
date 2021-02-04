//
// Created by thibaut on 01/02/2021.
//

#include "MapEditorState.hpp"
#include "../Resources/ResourceHolder.hpp"
#include <iostream>

MapEditorState::MapEditorState(StateStack &stack, Context context)
        : State(stack, context)
        , map(map::mode::mapEditor, context.textures.get(Textures::Map),  context.window.getSize()){
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
        switchTool(event);
    }

    if (event.type == sf::Event::MouseMoved){

        sf::Vector2i position = sf::Mouse::getPosition(getContext().window);
        sf::Vector2u  size = getContext().window.getSize();
        if (position.x < size.x*0.8f and position.x >= 0 and position.y >= size.y*0.1 and position.y < size.y*0.9 ) // rectangle contenant la map

            map.handleEvent(event, getContext().window);
    }
    return false;
}

void MapEditorState::switchTool(const sf::Event& event){
    switch (event.key.code){

        case sf::Keyboard::Numpad0 : map.setGroundSelection(Textures::ground::Grass); break;

        case sf::Keyboard::Numpad1 : map.setGroundSelection(Textures::ground::Sand); break;

        case sf::Keyboard::Numpad2 : map.setGroundSelection(Textures::ground::Wood); break;

        case sf::Keyboard::Numpad3 : map.setGroundSelection(Textures::ground::Water); break;

        case sf::Keyboard::Numpad4 : map.setGroundSelection(Textures::ground::Wall); break;

        case sf::Keyboard::Numpad7 : map.setRotation(0.f); break;

        case sf::Keyboard::Numpad8 : map.setRotation(90.f); break;

        case sf::Keyboard::Numpad9 : map.setRotation(-1.f); break;

        case sf::Keyboard::S : map.save("data/Maps/test.map"); break;

        case sf::Keyboard::L : map.load("data/Maps/test.map"); break;

        case sf::Keyboard::A : map.setTool(map::tool::standard); break;

        case sf::Keyboard::Z : map.setTool(map::tool::square3); break;

        case sf::Keyboard::E : map.setTool(map::tool::circle5); break;

        case sf::Keyboard::R : map.setTool(map::tool::fill); break;

        case sf::Keyboard::U : map.Undo(); break;

        default : std::cout<<"default way : "<<event.key.code<<"\n"; break;

    }
}
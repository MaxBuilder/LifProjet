//
// Created by thibaut on 01/02/2021.
//

#include "TilesMap.hpp"
#include <cassert>
#include <iostream>

TilesMap::TilesMap(mapMode::mode type,const sf::Texture &text){
    texture = text;
    ground_selection = Textures::ground::Grass;
    switch (type) {
        case mapMode::mapEditor :
            origin = sf::Vector2i(0,108);
            size = 24;
            break;
        case mapMode::game :
            origin = sf::Vector2i(0,0);
            size = 30;
            break;
        case mapMode::preview : break;
    }
    for(int y = 0; y < 36; y++){
        for(int x = 0; x < 64; x++){
            Tile *tile = &grid_id[x][y];
            tile->getSprite().setTexture(texture);

            setSpriteRect(grid_id[x][y].getGround(), *tile);

            tile->getSprite().setPosition(origin.x+float(x)*24,origin.y+float(y)*24);
            tile->getSprite().setScale(1.6f,1.6f);
        }
    }
}

void TilesMap::setSpriteRect(Textures::ground::ID id, Tile& tile){
    tile.setGround(id);

    switch (id) {

        case Textures::ground::None :
            tile.getSprite().setTextureRect(sf::IntRect(0, 0, 15, 15));
            break;

        case Textures::ground::Grass :
            tile.getSprite().setTextureRect(sf::IntRect(15, 0, 15, 15));
            break;

        case Textures::ground::Sand :
            tile.getSprite().setTextureRect(sf::IntRect(30, 0, 15, 15));
            break;

        case Textures::ground::Wood :
            tile.getSprite().setTextureRect(sf::IntRect(45, 0, 15, 15));
            break;

        case Textures::ground::Water :
            tile.getSprite().setTextureRect(sf::IntRect(60, 0, 15, 15));
            break;

        case Textures::ground::Wall :
            tile.getSprite().setTextureRect(sf::IntRect(75, 0, 15, 15));
            break;
    }

}

Tile& TilesMap::getTile(int x, int y){
    assert(x >= 0 and y >= 0 and x < 64 and y < 36 );
    return grid_id[x][y];
}

void TilesMap::handleEvent(const sf::Event& event, const sf::RenderWindow& window){
    sf::Vector2i pos = sf::Mouse::getPosition(window);
    pos.y = pos.y - 108;
    pos.y = pos.y/24;
    pos.x = pos.x/24;

    if (sf::Mouse::isButtonPressed(sf::Mouse::Right))
        setSpriteRect(Textures::ground::None, grid_id[pos.x][pos.y]);

    else if (sf::Mouse::isButtonPressed(sf::Mouse::Left))
        setSpriteRect(ground_selection, grid_id[pos.x][pos.y]);

}

void TilesMap::setGroundSelection(Textures::ground::ID id){
    ground_selection = id;
}

void TilesMap::draw(sf::RenderTarget& target, sf::RenderStates states) const{

    states.transform *= getTransform();

    for(int y = 0; y < 36; y++) {
        for (int x = 0; x < 64; x++) {

            target.draw(grid_id[x][y].getConstSprite(), states);

        }
    }
}
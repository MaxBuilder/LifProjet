//
// Created by thibaut on 01/02/2021.
//

#include "TilesMap.hpp"
#include <cassert>
#include <cstdlib>

TilesMap::TilesMap(mapMode::mode type,const sf::Texture &text){
    texture = text;
    rotate = 0;
    ground_selection = Textures::ground::Grass;

    switch (type) {
        case mapMode::mapEditor :
            size = 24;
            origin = sf::Vector2i(size/2,108+size/2);
            break;
        case mapMode::game :
            size = 30;
            origin = sf::Vector2i(size/2,size/2);
            break;
        case mapMode::preview : break;
    }

    for(int y = 0; y < 36; y++){
        for(int x = 0; x < 64; x++){
            Tile *tile = &grid_id[x][y];

            tile->getSprite().setTexture(texture);
            tile->getSprite().setScale(size/15.f,size/15.f);
            tile->getSprite().setOrigin(15.f/2.f,15.f/2.f);
            tile->getSprite().setPosition(origin.x+float(x)*size,origin.y+float(y)*size);

            setSpriteRect(0 ,grid_id[x][y].getGround(), *tile);

        }
    }
}

void TilesMap::setSpriteRect(float rotation, Textures::ground::ID id, Tile& tile){
    tile.setGround(id);

    if (rotation >= 0) tile.setRotation(rotation);

    else tile.setRotation((static_cast<float>(std::rand()%3)*90));


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
    pos.y = pos.y/size;
    pos.x = pos.x/size;

    if (sf::Mouse::isButtonPressed(sf::Mouse::Right))
        setSpriteRect(rotate,Textures::ground::None, grid_id[pos.x][pos.y]);

    else if (sf::Mouse::isButtonPressed(sf::Mouse::Left))
        setSpriteRect(rotate, ground_selection, grid_id[pos.x][pos.y]);

}

void TilesMap::setGroundSelection(Textures::ground::ID id){
    ground_selection = id;
}

void TilesMap::setRotation(float rotation){
    assert(rotation == 0 or rotation == 90 or rotation == -1);
    rotate = rotation;
}

void TilesMap::save(const std::string &file) const{

    std::ofstream wf(file, std::ios::out | std::ios::binary);
    if(!wf) {
        std::cout << "Cannot open file!" << std::endl;
        return;
    }

    for(const auto & x : grid_id) {
        for (const auto & y : x) {

            wf.write((char *) &(y.ground), sizeof(Textures::ground::ID));
            wf.write((char *) &(y.rotate), sizeof(float));
        }
    }
    wf.close();

}

void TilesMap::load(const std::string &file) {

    std::ifstream rf(file, std::ios::out | std::ios::binary);
    if(!rf) {
        std::cout << "Cannot open file!" << std::endl;
        return ;
    }

    for(auto & x : grid_id) {
        for (auto & tile : x) {

            rf.read((char *) &(tile.ground), sizeof(Textures::ground::ID));
            rf.read((char *) &(tile.rotate), sizeof(float));

            setSpriteRect(tile.rotate, tile.ground, tile);
        }
    }
    rf.close();


}

void TilesMap::draw(sf::RenderTarget& target, sf::RenderStates states) const{

    states.transform *= getTransform();

    for(int y = 0; y < 36; y++) {
        for (const auto & x : grid_id) {

            target.draw(x[y].getConstSprite(), states);

        }
    }
}
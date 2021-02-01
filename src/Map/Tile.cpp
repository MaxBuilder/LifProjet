//
// Created by thibaut on 01/02/2021.
//

#include "Tile.hpp"
#include <cassert>

Tile::Tile() {
    ground = Textures::ground::None;
    building = Textures::building::None;
    moveSpeed = 1;
    crossable = false;
}

sf::Sprite& Tile::getSprite(){
    return sprite;
}

sf::Sprite Tile::getConstSprite() const{
    return sprite;
}

Textures::ground::ID Tile::getGround() const{
    return ground;
}

Textures::building::ID Tile::getBuilding() const{
    return building;
}

int Tile::getMoveSpeed() const{
    return moveSpeed;
}

bool Tile::isCrossable() const{
    return crossable;
}

void Tile::setGround(const Textures::ground::ID &id){
    ground = id;
}

void Tile::setBuilding(const Textures::building::ID &id){
    building = id;
}

void Tile::setMoveSpeed(const int &speed){
    assert(speed >= 0);
    moveSpeed = speed;
}

void Tile::setcrossable(const bool &cross){
    crossable = cross;

}
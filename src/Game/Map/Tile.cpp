//
// Created by thibaut on 01/02/2021.
//

#include "Tile.hpp"
#include <cassert>

Tile::Tile(){
    ground = Textures::ground::None;
    rotate = 0;
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

float Tile::getMoveSpeed() const{
    return moveSpeed;
}

bool Tile::isCrossable() const{
    return crossable;
}

void Tile::setGround(const Textures::ground::ID &id){
    ground = id;
}

void Tile::setMoveSpeed(const float &speed){
    assert(speed >= 0);
    moveSpeed = speed;
}

void Tile::setcrossable(const bool &cross){
    crossable = cross;

}

float Tile::getRotation() const{
    return rotate;
}

void Tile::setRotation(const float &rotation){
    assert(rotation == 0 or rotation == 90 or rotation == 180 or rotation == 270);
    rotate = rotation;
    sprite.setRotation(rotation);
}

BuildMap::BuildMap(Textures::building::ID ID,sf::IntRect position, float rotation ){
    mId  = ID;
    mPosition = position;
    mRotation = rotation;
}

Textures::building::ID BuildMap::getID() const{
    return mId;
}

sf::IntRect BuildMap::getPosition() const{
    return mPosition;
}

sf::Sprite& BuildMap::getSprite(){
    return mSprite;
}

sf::Sprite BuildMap::getConstSprite() const{
    return mSprite;
}
void BuildMap::setPosition(sf::IntRect rect) {
    mPosition = rect;
}
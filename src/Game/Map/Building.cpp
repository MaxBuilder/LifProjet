//
// Created by 33771 on 18/02/2021.
//

#include "Building.hpp"

Building::Building(Textures::Building::ID ID, sf::IntRect position, float rotation ){
    mId  = ID;
    mPosition = position;
    mRotation = rotation;
}

Textures::Building::ID Building::getID() const{
    return mId;
}

sf::IntRect Building::getPosition() const{
    return mPosition;
}

sf::Sprite& Building::getSprite(){
    return mSprite;
}

sf::Sprite Building::getConstSprite() const{
    return mSprite;
}
void Building::setPosition(sf::IntRect rect) {
    mPosition = rect;
}
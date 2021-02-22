//
// Created by 33771 on 18/02/2021.
//

#include "BuildMap.hpp"

BuildMap::BuildMap(Textures::Building::ID ID, sf::IntRect position, float rotation ){
    mId  = ID;
    mPosition = position;
    mRotation = rotation;
}

Textures::Building::ID BuildMap::getID() const{
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
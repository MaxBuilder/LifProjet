//
// Created by 33771 on 18/02/2021.
//

#include "Building.hpp"

Building::Building(Buildings::ID ID, sf::IntRect position){
    mId  = ID;
    mPosition = position;
}

Buildings::ID Building::getID() const{
    return mId;
}

sf::IntRect Building::getPosition() const{
    return mPosition;
}

void Building::setPosition(sf::IntRect rect) {
    mPosition = rect;
}
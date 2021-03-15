//
// Created by 33771 on 18/02/2021.
//

#include "BuildInfo.hpp"

BuildInfo::BuildInfo(Buildings::ID ID, sf::IntRect position){
    mId  = ID;
    mPosition = position;
}

Buildings::ID BuildInfo::getID() const{
    return mId;
}

sf::IntRect BuildInfo::getPosition() const{
    return mPosition;
}

void BuildInfo::setPosition(sf::IntRect rect) {
    mPosition = rect;
}
//
// Created by 33771 on 18/02/2021.
//
#include "EntityInfo.hpp"
#include "BuildInfo.hpp"

BuildInfo::BuildInfo(EntityInfo::ID ID,EntityInfo::Team team, sf::IntRect position){
    mId  = ID;
    mTeam = team;
    mPosition = position;
}

EntityInfo::ID BuildInfo::getID() const{
    return mId;
}

EntityInfo::Team BuildInfo::getTeam() const {
    return mTeam;
}

sf::IntRect BuildInfo::getPosition() const{
    return mPosition;
}

void BuildInfo::setPosition(sf::IntRect rect) {
    mPosition = rect;
}
//
// Created by thibaut on 28/03/2021.
//

#include "EntityInfo.hpp"

EntityInfo::EntityInfo(sf::Vector2f position, EntityInfo::ID id, EntityInfo::Team team,EntityInfo::Type type){
    mPosition = position;
    mType = type;
    mId = id;
    mTeam = team;
}

sf::Vector2f EntityInfo::getPosition() const{
    return mPosition;
}

EntityInfo::ID EntityInfo::getID() const{
    return mId;
}

EntityInfo::Team EntityInfo::getTeam() const{
    return mTeam;
}

EntityInfo::Type EntityInfo::getType() const {
    return mType;
}
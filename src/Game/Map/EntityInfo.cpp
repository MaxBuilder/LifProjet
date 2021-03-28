//
// Created by thibaut on 28/03/2021.
//

#include "EntityInfo.hpp"

EntityInfo::EntityInfo(sf::Vector2f position, Editor::Entity type, Editor::Tool team){
    mPosition = position;
    mType = type;
    mTeam = team;
}

sf::Vector2f EntityInfo::getPosition() const{
    return mPosition;
}

Editor::Entity EntityInfo::getType() const{
    return mType;
}

Editor::Tool EntityInfo::getTeam() const{
    return mTeam;
}
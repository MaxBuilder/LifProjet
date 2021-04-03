//
// Created by thibaut on 12/03/2021.
//

#include "Building.hpp"
#include <iostream>

Building::Building(EntityInfo::ID ID,EntityInfo::Team team, sf::IntRect position,  CommandQueue& commandQueue) :
   Entity(0,EntityInfo::Team::Blue, commandQueue)
 {

    mPosition = position;
    setPosition(float(mPosition.left)*20+float(mPosition.width)*20/2,float(mPosition.top)*20+float(mPosition.width)*20/2);

    switch (ID){
        case EntityInfo::ID::Castle :
            mBorder = 40;
            mHitPoints = 500;
            mTeam = team;
            mRange = 150;
            mBonusFlag = ID;
            mTextureId.y = 53; mTextureId.x = 0;
            break;

        case EntityInfo::ID::Village :
            mBorder = 27;
            mHitPoints = 300;
            mTeam =team;
            mRange = 100;
            mBonusFlag = ID;
            mTextureId.y = 48; mTextureId.x = 0;
            break;


        case  EntityInfo::ID::Barrier :
            mBorder = 15;
            mHitPoints = 200;
            mTeam = team;
            mRange = 0;
            mBonusFlag = ID;
            mTextureId.y = 49; mTextureId.x = 2;
            break;

        default:
            break;
    }

    mZone.setRadius(mRange);
    mZone.setOrigin(mRange, mRange);
    if(mTeam == EntityInfo::Team::Blue)
        mZone.setOutlineColor(sf::Color::Blue);
    else
        mZone.setOutlineColor(sf::Color::Red);
    mZone.setOutlineThickness(1);
    mZone.setFillColor(sf::Color::Transparent);
}

float Building::getRange() const{
    return mRange;
}

EntityInfo::ID Building::getBonusFlag() const{
    return mBonusFlag;
}

void Building::drawCurrent(sf::RenderTarget& target, sf::RenderStates states) const {
    if (not isDestroyed())
        target.draw(mZone, states);
}

sf::Vector2i Building::getOnMapPosition() const{
    return sf::Vector2i(mPosition.left,mPosition.top);
}

sf::Vector2i Building::getOnMapSize() const{
    return sf::Vector2i(mPosition.width,mPosition.height);
}

sf::Vector2i Building::getMapId() const{
    return mTextureId;
}

void Building::updateCurrent(){

}
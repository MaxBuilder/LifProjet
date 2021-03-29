//
// Created by thibaut on 12/03/2021.
//

#include "Building.hpp"
#include <iostream>

Building::Building(Buildings::ID ID, sf::IntRect position) :
 Entity(0,Entity::Team::BlueTeam){

    mPosition = {position.left*20,position.top*20,position.width*20,position.height*20};
    setPosition(mPosition.left+mPosition.width/2,mPosition.top+mPosition.width/2);

    switch (ID){
        case Buildings::BlueCastle :
            mBorder = 40;
            mHitPoints = 500;
            mTeam = Entity::BlueTeam;
            mRange = 150;
            mBonusFlag = Entity::Castle;
            break;

        case Buildings::RedCastle :
            mBorder = 40;
            mHitPoints = 500;
            mTeam = Entity::RedTeam;
            mRange = 150;
            mBonusFlag = Entity::Castle;
            break;

        case Buildings::BlueVillage :
            mBorder = 27;
            mHitPoints = 300;
            mTeam = Entity::BlueTeam;
            mRange = 100;
            mBonusFlag = Entity::Village;
            break;

        case Buildings::RedVillage :
            mBorder = 27;
            mHitPoints = 300;
            mTeam = Entity::RedTeam;
            mRange = 100;
            mBonusFlag = Entity::Village;
            break;

        case Buildings::BlueBarrier :
            mBorder = 15;
            mHitPoints = 200;
            mTeam = Entity::BlueTeam;
            mRange = 0;
            mBonusFlag = Entity::None;
            break;

        case Buildings::RedBarrier :
            mBorder = 15;
            mHitPoints = 200;
            mTeam = Entity::RedTeam;
            mRange = 0;
            mBonusFlag = Entity::None;
            break;

        default:
            mHitPoints = 0;
            mTeam = Entity::BlueTeam;
            mRange = 0;
            mBonusFlag = Entity::None;
            break;
    }

    zone.setRadius(mRange);
    zone.setOrigin(mRange,mRange);
    if(mTeam == Entity::BlueTeam)
        zone.setOutlineColor(sf::Color::Blue);
    else
        zone.setOutlineColor(sf::Color::Red);
    zone.setOutlineThickness(1);
    zone.setFillColor(sf::Color::Transparent);
}

float Building::getRange() const{
    return mRange;
}

Entity::Bonus Building::getBonusFlag() const{
    return mBonusFlag;
}

void Building::drawCurrent(sf::RenderTarget& target, sf::RenderStates states) const {
    if (not isDestroyed())
        target.draw(zone, states);
}
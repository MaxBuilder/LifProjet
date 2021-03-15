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
            mHitPoints = 500;
            mTeam = Entity::BlueTeam;
            mRange = 200;
            mBonusFlag = Entity::Castle;
            break;

        case Buildings::RedCastle :
            mHitPoints = 500;
            mTeam = Entity::RedTeam;
            mRange = 200;
            mBonusFlag = Entity::Castle;
            break;

        case Buildings::BlueVillage :
            mHitPoints = 300;
            mTeam = Entity::BlueTeam;
            mRange = 150;
            mBonusFlag = Entity::Village;
            break;

        case Buildings::RedVillage :
            mHitPoints = 300;
            mTeam = Entity::RedTeam;
            mRange = 150;
            mBonusFlag = Entity::Village;
            break;

        case Buildings::BlueBarrier :
            mHitPoints = 200;
            mTeam = Entity::BlueTeam;
            mRange = 0;
            mBonusFlag = Entity::None;
            break;

        case Buildings::RedBarrier :
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
    target.draw(zone, states);
}
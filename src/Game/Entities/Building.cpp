//
// Created by thibaut on 12/03/2021.
//

#include "Building.hpp"
#include <iostream>

Building::Building(Buildings::ID ID, sf::IntRect position,  CommandQueue& commandQueue) :
   Entity(0,Entity::Team::BlueTeam, commandQueue)
 {

    mPosition = position;
    setPosition(float(mPosition.left)*20+float(mPosition.width)*20/2,float(mPosition.top)*20+float(mPosition.width)*20/2);

    switch (ID){
        case Buildings::BlueCastle :
            mBorder = 40;
            mHitPoints = 500;
            mTeam = Entity::BlueTeam;
            mRange = 150;
            mBonusFlag = Entity::Castle;
            mTextureId.y = 53; mTextureId.x = 0;
            break;

        case Buildings::RedCastle :
            mBorder = 40;
            mHitPoints = 500;
            mTeam = Entity::RedTeam;
            mRange = 150;
            mBonusFlag = Entity::Castle;
            mTextureId.y = 50; mTextureId.x = 0;
            break;

        case Buildings::BlueVillage :
            mBorder = 27;
            mHitPoints = 300;
            mTeam = Entity::BlueTeam;
            mRange = 100;
            mBonusFlag = Entity::Village;
            mTextureId.y = 48; mTextureId.x = 0;
            break;

        case Buildings::RedVillage :
            mBorder = 27;
            mHitPoints = 300;
            mTeam = Entity::RedTeam;
            mRange = 100;
            mBonusFlag = Entity::Village;
            mTextureId.y = 46; mTextureId.x = 0;
            break;

        case Buildings::BlueBarrier :
            mBorder = 15;
            mHitPoints = 200;
            mTeam = Entity::BlueTeam;
            mRange = 0;
            mBonusFlag = Entity::None;
            mTextureId.y = 49; mTextureId.x = 2;
            break;

        case Buildings::RedBarrier :
            mBorder = 15;
            mHitPoints = 200;
            mTeam = Entity::RedTeam;
            mRange = 0;
            mBonusFlag = Entity::None;
            mTextureId.y = 47; mTextureId.x = 2;
            break;

        default:
            mHitPoints = 0;
            mTeam = Entity::BlueTeam;
            mRange = 0;
            mBonusFlag = Entity::None;
            break;
    }

    mZone.setRadius(mRange);
    mZone.setOrigin(mRange, mRange);
    if(mTeam == Entity::BlueTeam)
        mZone.setOutlineColor(sf::Color::Blue);
    else
        mZone.setOutlineColor(sf::Color::Red);
    mZone.setOutlineThickness(1);
    mZone.setFillColor(sf::Color::Transparent);
}

float Building::getRange() const{
    return mRange;
}

Entity::Bonus Building::getBonusFlag() const{
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
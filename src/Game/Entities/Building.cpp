//
// Created by thibaut on 12/03/2021.
//

#include "Building.hpp"
#include <iostream>

Building::Building(EntityInfo::ID ID,EntityInfo::Team team, sf::Vector2f position, CommandQueue& commandQueue)
: Entity(0, team, commandQueue)
, previouslyHit(false)
{
    mPosition.left = position.x;
    mPosition.top = position.y;

    switch(ID) {
        case EntityInfo::ID::Castle :
            mBorder = 40;
            mHitPoints = 5000;
            mTeam = team;
            mRange = 150;
            mBonusFlag = ID;
            mTextureId.y = 53; mTextureId.x = 0;
            mPosition.width = 3; mPosition.height = 3;
            break;

        case EntityInfo::ID::Village :
            mBorder = 27;
            mHitPoints = 300;
            mTeam = team;
            mRange = 100;
            mBonusFlag = ID;
            mTextureId.y = 48; mTextureId.x = 0;
            mPosition.width = 2; mPosition.height = 2;
            break;


        case EntityInfo::ID::Barrier :
            mBorder = 15;
            mHitPoints = 200;
            mTeam = team;
            mRange = 0;
            mBonusFlag = ID;
            mTextureId.y = 49; mTextureId.x = 2;
            mPosition.width = 1; mPosition.height = 1;
            break;

        default:
            break;
    }
     setPosition(float(mPosition.left)*20+float(mPosition.width)*20/2,float(mPosition.top)*20+float(mPosition.width)*20/2);
     mMaxHintPoints = mHitPoints;
     prevHealth = mHitPoints;

    mZone.setRadius(mRange);
    mZone.setOrigin(mRange, mRange);
    if(mTeam == EntityInfo::Team::Blue)
        mZone.setOutlineColor(sf::Color::Blue);
    else
        mZone.setOutlineColor(sf::Color::Red);
    mZone.setOutlineThickness(1);
    mZone.setFillColor(sf::Color::Transparent);
    float blockSize = 20.f;

     backLife.setPosition(-blockSize*mPosition.width/2.f,-22);
     backLife.setSize(sf::Vector2f(blockSize*mPosition.width+1,5));
     backLife.setFillColor(sf::Color::Black);

     frontLife.setPosition(-blockSize*mPosition.width/2.f+1,-21);
     frontLife.setSize(sf::Vector2f(blockSize*mPosition.width-1,3));
     frontLife.setFillColor(sf::Color::Green);
}

float Building::getRange() const {
    return mRange;
}

EntityInfo::ID Building::getBonusFlag() const {
    return mBonusFlag;
}

void Building::drawCurrent(sf::RenderTarget& target, sf::RenderStates states) const {
    if (not isDestroyed()) {
        if (mHitPoints != mMaxHintPoints) {
            target.draw(backLife, states);
            target.draw(frontLife, states);
        }
        target.draw(mZone, states);
    }
}

sf::Vector2i Building::getOnMapPosition() const {
    return sf::Vector2i(mPosition.left,mPosition.top);
}

sf::Vector2i Building::getOnMapSize() const {
    return sf::Vector2i(mPosition.width,mPosition.height);
}

sf::Vector2i Building::getMapId() const {
    return mTextureId;
}

void Building::updateCurrent(sf::Time dt) {
    float blockSize = 20.f;
    float xlife = float(mHitPoints)/float(mMaxHintPoints);
    float lifeLength = blockSize*mPosition.width*xlife;

    frontLife.setSize(sf::Vector2f(lifeLength,3));

    if(xlife > 0.5f)
        frontLife.setFillColor(sf::Color::Green);
    else if(xlife > 0.25f)
        frontLife.setFillColor(sf::Color::Yellow);
    else frontLife.setFillColor(sf::Color::Red);

    if(mBonusFlag == EntityInfo::Castle) {
        if(mHitPoints < prevHealth and !previouslyHit) {
            mCommandQueue.push(Command(EntityInfo::Blue, 0, 0, CommandType::CastleAssaulted));
            previouslyHit = true;
            mBuildingTime = sf::seconds(0);
            prevHealth = mHitPoints;
        }
    }

    mBuildingTime += dt;

    if(mBuildingTime.asSeconds() > 5)
        previouslyHit = false;
}
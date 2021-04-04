//
// Created by thibaut on 12/03/2021.
//

#include "Building.hpp"
#include <iostream>

Building::Building(EntityInfo::ID ID,EntityInfo::Team team, sf::IntRect position,  CommandQueue& commandQueue) :
   Entity(0,EntityInfo::Team::Blue, commandQueue),
   frontLife(sf::Quads, 4),
   fontLife(sf::Quads, 4)
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
     mMaxHintPoints = mHitPoints;

    mZone.setRadius(mRange);
    mZone.setOrigin(mRange, mRange);
    if(mTeam == EntityInfo::Team::Blue)
        mZone.setOutlineColor(sf::Color::Blue);
    else
        mZone.setOutlineColor(sf::Color::Red);
    mZone.setOutlineThickness(1);
    mZone.setFillColor(sf::Color::Transparent);
    float blockSize = 20.f;

    frontLife[0].position = sf::Vector2f(-blockSize*mPosition.width/2.f,-22);
    frontLife[0].color = sf::Color::Green;
    frontLife[1].position = sf::Vector2f(blockSize*mPosition.width/2.f,-22);
    frontLife[1].color = sf::Color::Green;
    frontLife[2].position = sf::Vector2f(blockSize*mPosition.width/2.f,-18);
    frontLife[2].color = sf::Color::Green;
    frontLife[3].position = sf::Vector2f(-blockSize*mPosition.width/2.f,-18);
    frontLife[3].color = sf::Color::Green;

    fontLife[0].position = sf::Vector2f(-blockSize*mPosition.width/2.f -1,-23);
    fontLife[0].color = sf::Color::Black;
    fontLife[1].position = sf::Vector2f(blockSize*mPosition.width/2.f+1,-23);
    fontLife[1].color = sf::Color::Black;
    fontLife[2].position = sf::Vector2f(blockSize*mPosition.width/2.f+1,-17);
    fontLife[2].color = sf::Color::Black;
    fontLife[3].position = sf::Vector2f(-blockSize*mPosition.width/2.f -1,-17);
    fontLife[3].color = sf::Color::Black;
}

float Building::getRange() const{
    return mRange;
}

EntityInfo::ID Building::getBonusFlag() const{
    return mBonusFlag;
}

void Building::drawCurrent(sf::RenderTarget& target, sf::RenderStates states) const {
    if (not isDestroyed()) {
        if (mHitPoints != mMaxHintPoints) {
            target.draw(fontLife, states);
            target.draw(frontLife, states);
        }
        target.draw(mZone, states);
    }
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

void Building::updateCurrent(sf::Time dt){
    float blockSize = 20.f;
    float xlife = float(mHitPoints)/float(mMaxHintPoints);
    float lifeLength = blockSize*mPosition.width*xlife;

    frontLife[1].position = sf::Vector2f(-blockSize*mPosition.width/2.f + lifeLength,-22);
    frontLife[2].position = sf::Vector2f(-blockSize*mPosition.width/2.f + lifeLength,-18);

    if(xlife > 0.5f){
        frontLife[0].color = sf::Color::Green;
        frontLife[1].color = sf::Color::Green;
        frontLife[2].color = sf::Color::Green;
        frontLife[3].color = sf::Color::Green;
    }else if(xlife > 0.25f){
        frontLife[0].color = sf::Color::Yellow;
        frontLife[1].color = sf::Color::Yellow;
        frontLife[2].color = sf::Color::Yellow;
        frontLife[3].color = sf::Color::Yellow;
    }else{
        frontLife[0].color = sf::Color::Red;
        frontLife[1].color = sf::Color::Red;
        frontLife[2].color = sf::Color::Red;
        frontLife[3].color = sf::Color::Red;
    }

}
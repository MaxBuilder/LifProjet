//
// Created by 33771 on 23/02/2021.
//

#include "Soldier.hpp"

#include <iostream>

// A faire : ajouter les tables de données pour les entités
Soldier::Soldier(Team team, const TextureHolder& textures, const FontHolder& fonts, TilesMap &map, bool big)
: Entity(100,team)
, mDirection(sf::Vector2f(0, 0))
, mOrigin(sf::Vector2f(0, 0))
, mSprite(textures.get(Textures::EntitySoldier))
, mGlow(textures.get(Textures::EntityGlow))
, mSpeedBase(15)
, mSpeedBonus(0)
, mBonus(Entity::None)
, mDamages(20)
, mTargeted(nullptr)
, mAction(Moving)
, mTravelled(0.f)
, isBigBitch(big)
, isAvailable(true)
{
    mMap = std::make_shared<TilesMap>(map);
    // Fix origin and texture selection
    mGlow.setTextureRect(sf::IntRect(0,0,32,32));
    // setOrigin(getPosition().x + 10, getPosition().y + 10);
    mTeam == BlueTeam ? mSpriteRect= sf::IntRect(32,0,32,32) : mSpriteRect = sf::IntRect(32,32,32,32);
    mSprite.setTextureRect(mSpriteRect);
    setOrigin(mSprite.getLocalBounds().width/2.f,mSprite.getLocalBounds().height/2.f);
    mSpriteTime = sf::milliseconds(0);

    if(isBigBitch) heal(100); // HP to 200 for big entities

    mLife.setFont(fonts.get(Fonts::Main));
    mLife.setFillColor(sf::Color::Black);
    mLife.setCharacterSize(10u);
}

void Soldier::drawCurrent(sf::RenderTarget &target, sf::RenderStates states) const {
    target.draw(mSprite, states);
    target.draw(mGlow,states);
    sf::Vertex line[] = {
            sf::Vertex(sf::Vector2f(getPosition()), sf::Color::Red),
            sf::Vertex(sf::Vector2f(getPosition() + mDirection * 10.f), sf::Color::Red)
    };
    target.draw(line, 2, sf::Lines);
    target.draw(mLife);
}

void Soldier::updateCurrent(sf::Time dt) {
    mSpriteTime = sf::milliseconds(dt.asMilliseconds()+mSpriteTime.asMilliseconds());
    if (mSpriteTime.asMilliseconds() > 200){
        if(getHitPoints() > 0)
            mSpriteRect.left += 32;
        else
            mSpriteRect.left = 0;
        if (mDirection.x > 0 and mSpriteRect.left >= 32*7){
            mSpriteRect.left = 32;
        }else if (mDirection.x < 0 and  mSpriteRect.left >= 32*13 )
            mSpriteRect.left = 32*7;
        if(mDirection.x == 0 and mTargeted != nullptr and getHitPoints() > 0 ){
            if (mTargeted->getPosition().x < getPosition().x)
                mSpriteRect.left = 32*7;
            else
                mSpriteRect.left = 32;
        }
        mSpriteTime = sf::milliseconds(0);
        mSprite.setTextureRect(mSpriteRect);
    }
    mTeam == BlueTeam ? updateDefense(dt) : updateAttack(dt);
    mLife.setString(std::to_string(getHitPoints()));
    mLife.setPosition(getPosition()+sf::Vector2f(0,-20));
    centerOrigin(mLife);
}

void Soldier::updateAttack(sf::Time dt) {
    if(mAction == None or isDestroyed()) return;
    else if(mAction == Override) {
        moveIt(mDirection * dt.asSeconds() * (80.f+mSpeedBonus));
        return;
    }
    if(mAction == Moving) {
        if(mTargeted == nullptr) {
            setDirection(sf::Vector2f(1, 0));
            moveIt(mDirection * dt.asSeconds() * (mSpeedBonus+mSpeedBase));
        }
        else {
            if(mTargeted->isBigBitch) {
                isAvailable = false;
                mAction = Calling;
            }
            else {
                mAction = Seeking;
                isAvailable = false;
            }
        }
    }
    else if(mAction == Seeking) {
        if(mTargeted == nullptr) {
            mAction = Moving;
            mSpeedBase = 15;
            isAvailable = true;
            return;
        }

        if(distance(getPosition(), mTargeted->getPosition()) < 20) {
            mAction = Attacking;
            mDirection = sf::Vector2f(0, 0);
            mEntityClock.restart();
            mSpeedBase = 15;
            return;
        }

        seekTarget();
        moveIt(mDirection * dt.asSeconds() * (mSpeedBonus+mSpeedBase));
    }
    else if(mAction == Attacking) {
        if(distance(getPosition(), mTargeted->getPosition()) >= 30) {
            mAction = Seeking;
            return;
        }

        attackTarget();

        if(mTargeted->isDestroyed()) {
            mAction = Moving;
            isAvailable = true;
        }
    }
    else if(mAction == Calling) {
        if(mTargeted->isDestroyed()) {
            isAvailable = true;
            mAction = Moving;
            mTargeted = nullptr;
        }
    }
    else if(mAction == Helping) {
        if(distance(getPosition(), mTargeted->getPosition()) > 30) {
            seekTarget();
            moveIt(mDirection * dt.asSeconds() * (mSpeedBonus+mSpeedBase));
        }
        else {
            mAction = Attacking;
            mTargeted->setAction(Attacking);
            mTargeted = mTargeted->getTarget();
        }
    }
}

void Soldier::updateDefense(sf::Time dt) {
    if(mAction == None or isDestroyed()) return;
    else if(mAction == Override) {
        moveIt(mDirection * dt.asSeconds() * (80.f+mSpeedBonus));
        return;
    }
    if(mAction == Moving) {
        if(mTravelled == 0)
            setDirection(randomDirection());

        if(mTargeted == nullptr) {
            roam(dt);
        }
        else {
            mAction = Seeking;
            mTravelled = 0;
        }
    }
    else if(mAction == Seeking) {
        if(mTargeted == nullptr) {
            mAction = Moving;
            mSpeedBase = 15;
            return;
        }

        if(distance(getPosition(), mTargeted->getPosition()) < 20) {
            mAction = Attacking;
            mDirection = sf::Vector2f(0, 0);
            mEntityClock.restart();
            return;
        }
        /*
        if(distance(mOrigin, mTargeted->getPosition()) > 100) {
            mAction = Moving;
            return;
        }*/

        seekTarget();
        moveIt(mDirection * dt.asSeconds() * (mSpeedBonus+mSpeedBase));
    }
    else if(mAction == Attacking) {
        if(distance(getPosition(), mTargeted->getPosition()) >= 30) {
            mAction = Seeking;
            return;
        }

        attackTarget();

        // Add leave condition here (flee or stay)
        if(mTargeted->isDestroyed()) {
            mAction = Moving;
        }
    }
}

void Soldier::helpRequested(Soldier * ally) {
    //mTargeted = ally;
    mDirection = sf::Vector2f(0, 0);
    isAvailable = false;
}

void Soldier::helpAlly(Soldier * ally) {
    mAction = Helping;
    mTargeted = ally;
    isAvailable = false;
}

void Soldier::attackTarget() {
    if(mEntityClock.getElapsedTime().asSeconds() > 1) {
        mTargeted->damage(mDamages);
        mEntityClock.restart();
    }
    if(mTargeted->isDestroyed())
        mTargeted->remove();
}

void Soldier::roam(sf::Time dt) {
    if(distance(getPosition(), mOrigin) > 50) { // If too far from origin, go back
        mDistance = 10;
        mTravelled = 0;
        setDirection(mOrigin - getPosition());
        moveIt(mDirection * dt.asSeconds() * (mSpeedBonus+mSpeedBase));
        return;
    }
    if(mTravelled < (float)mDistance) { // If there is still distance to travel
        sf::Vector2f prev = getPosition();
        sf::Vector2f tomoveIt = mDirection * dt.asSeconds() * (10.f+mSpeedBonus);
        moveIt(tomoveIt);
        mTravelled += distance(prev, getPosition());
        return;
    }
    // If distance has been travelled, find a new one
    mTravelled = 0;
    setDirection(randomDirection());
}

sf::Vector2f Soldier::randomDirection() {
    mDistance = Random::Generate(10.f, 60.f);
    return sf::Vector2f(Random::Generate(-10.f, 10.f), Random::Generate(-10.f, 10.f));
}

void Soldier::seekTarget() {
    sf::Vector2f target = mTargeted->getPosition();
    target = target - getPosition();
    mDirection = target / norm(target);
    mSpeedBase = 30;
}

void Soldier::fleeTarget() {
    if(mTargeted != nullptr) {
        sf::Vector2f target = mTargeted->getPosition();
        target = getPosition() - target;
        mDirection = target / norm(target);
        mSpeedBase = 30;
    }
    else mSpeedBase = 15;
}

Soldier::Action Soldier::getAction() {
    return mAction;
}

void Soldier::setAction(Action act) {
    mAction = act;
}

void Soldier::remove() {
    Entity::remove();
    mSprite.setTextureRect(sf::IntRect(0, 0, 32, 32));
}

void Soldier::setDirection(sf::Vector2f velocity) {
    float norme = norm(velocity);
    norme > 0 ? mDirection = velocity / norme : mDirection = sf::Vector2f(0, 0);
}

void Soldier::setDirection(float vx, float vy) {
    float norme = norm(sf::Vector2f(vx, vy));
    norme > 0 ? mDirection = sf::Vector2f(vx, vy) / norme : mDirection = sf::Vector2f(0, 0);
}

sf::Vector2f Soldier::getDirection() const {
    return mDirection;
}

float Soldier::getSpeed() const {
    return mSpeedBase+mSpeedBonus;
}

void Soldier::setSpeed(float speed) {
    mSpeedBase = speed;
}

void Soldier::setTarget(Soldier* target) {
    mTargeted = target;
}

void Soldier::dropTarget() {
    mTargeted = nullptr;
}

Soldier* Soldier::getTarget() {
    return mTargeted;
}

void Soldier::init() {
    mOrigin = getPosition();
}

void Soldier::resetTravelledDistance() {
    mTravelled = 0;
}

sf::Vector2f Soldier::getOrigin() {
    return mOrigin;
}

void Soldier::changeBonus(Entity::Bonus bonus) {
    if(bonus == Entity::Castle){
        mGlow.setTextureRect(sf::IntRect(64,0,32,32));
        mBonus = bonus;
        mSpeedBonus = 15;
        mDamages = 50;
    }else if(bonus == Entity::Village and mBonus == Entity::None){
        mGlow.setTextureRect(sf::IntRect(32,0,32,32));
        mBonus = bonus;
        mSpeedBonus = 15;
        mDamages = 40;
    }else if(bonus == Entity::None){
        mGlow.setTextureRect(sf::IntRect(0,0,32,32));
        mBonus = bonus;
        mSpeedBonus = 0;
        mDamages = 20;
    }
}
Entity::Bonus Soldier::getBonus(){
    return mBonus;
}

void Soldier::moveIt(sf::Vector2f dpl){
    sf::Vector2f point = (getPosition()+dpl);
    if(mDirection.x > 0)
        point.x += mSprite.getLocalBounds().width/4.f;
    else
        point.x -= mSprite.getLocalBounds().width/4.f;

    if(mDirection.y > 0)
        point.y += mSprite.getLocalBounds().height/4.f;
    else
        point.y -= mSprite.getLocalBounds().height/4.f;

    sf::Vector2i pos = sf::Vector2i(point.x/20, point.y/20);
    if (mMap->getTile(pos.x,pos.y).isCrossable())
        move(dpl);
    else{
        sf::Vector2i pos1 = sf::Vector2i(point.x/20, (point.y-dpl.y)/20);
        sf::Vector2i pos2 = sf::Vector2i((point.x-dpl.x)/20, point.y/20);
        if (mMap->getTile(pos1.x,pos1.y).isCrossable())
            move(dpl.x,0);
        else if(mMap->getTile(pos2.x,pos2.y).isCrossable())
            move(0,dpl.y);
        else {
            // mDirection = -mDirection;
            // moveIt(-dpl);
        }
    }




}
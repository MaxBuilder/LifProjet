//
// Created by 33771 on 23/02/2021.
//

#include "Soldier.hpp"

#include <iostream>

// A faire : ajouter les tables de données pour les entités
Soldier::Soldier(int id, Team team, const TextureHolder& textures, const FontHolder& fonts, AstarAlgo& Astar, CommandQueue& commandQueue, bool big)
: Entity(100,team)
, mId(id)
, mVelocity(0,0)
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
//, isAvailable(true)
, mCommandQueue(commandQueue)
, mTargetInSight(0)
, mAllyInSight(0)
, mLeader(nullptr)
, nbRequested(0)
, nbResponse(0)
, prev(false)
, mSquadSize(0)
{
    mAstar = std::make_shared<AstarAlgo>(Astar);
    float blockSize = 20.f; // à modifier pour rendre dynamique
    // Fix origin and texture selection
    mGlow.setTextureRect(sf::IntRect(0,0,32,32));
    mTeam == BlueTeam ? mSpriteRect= sf::IntRect(32,0,32,32) : mSpriteRect = sf::IntRect(32,32,32,32);
    mSprite.setTextureRect(mSpriteRect);
    mSprite.setScale(blockSize/mSprite.getLocalBounds().width,blockSize/mSprite.getLocalBounds().height);
    mGlow.setScale(blockSize/mSprite.getLocalBounds().width,blockSize/mSprite.getLocalBounds().height);
    setOrigin(blockSize/2.f,blockSize/2.f);
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

    if(mAction != Moving)
        mPath.clear();

    if(mAction == None or isDestroyed()) return;
    else if(mAction == Override) {
        setVelocity(mDirection * dt.asSeconds() * (80.f+mSpeedBonus));
        return;
    }
    if(mAction == Moving) {
        if(mTargetInSight > 1 and mTargeted == nullptr) {
            // Lancement du signal et mise en groupe
            mCommandQueue.push(Command(true, mId, 9999, CommandType::MakeTeam));
            mAction = Calling;
            nbResponse = 0;
            nbRequested = 0;
        }

        if(mTargeted == nullptr) {
            if(mPath.empty()) {
                mAstarDuration.restart();
                mAstar->getPath(getPosition(),sf::Vector2f(57,5),mPath,2) ;
                //std::cout << "Astar Duration :" << mAstarDuration.getElapsedTime().asMicroseconds() << std::endl;
            }
            sf::Vector2f &target = mPath.back();
            if (distance(getPosition(), target) < 2 )
                mPath.pop_back();
            setDirection((target-getPosition())/norm(target-getPosition()));
            setVelocity(mDirection * dt.asSeconds() * (mSpeedBonus+mSpeedBase));
        }
        else {
            if(mTargeted->isBigBitch) {
                //isAvailable = false;
                mAction = Calling;
            }
            else {
                mAction = Seeking;
                //isAvailable = false;
            }
        }
    }
    else if(mAction == Seeking) {
        if(mTargeted == nullptr) {
            mAction = Moving;
            mSpeedBase = 15;
            //isAvailable = true;
            return;
        }

        if(distance(getPosition(), mTargeted->getPosition()) < 20) {
            mAction = Attacking;
            mDirection = sf::Vector2f(0,0);
            mEntityClock.restart();
            mSpeedBase = 15;
            setVelocity(mDirection * dt.asSeconds() * (mSpeedBonus+mSpeedBase));
            return;
        }

        seekTarget();
        setVelocity(mDirection * dt.asSeconds() * (mSpeedBonus+mSpeedBase));
    }
    else if(mAction == Attacking) {
        if(distance(getPosition(), mTargeted->getPosition()) >= 30) {
            mAction = Seeking;
            return;
        }

        attackTarget();

        if(mTargeted->isDestroyed()) {
            mAction = Moving;
            //isAvailable = true;
        }
    }
    else if(mAction == Calling) {
        if(nbRequested != 0 and nbResponse == nbRequested) {
            mAction = Leading;
            nbResponse = 0;
            nbRequested = 0;
            //std::cout << "Squad size " << mSquadSize << " " << mSquadIds.size() << std::endl;
        }
        else if(mTargeted != nullptr)
            mAction = Seeking;
        mVelocity = sf::Vector2f(0, 0);
    }
    else if(mAction == Leading) {
        if(nbResponse == mSquadSize) {
            // Attack
            //std::cout << "Assault " << std::endl;
            for (auto id : mSquadIds)
                mCommandQueue.push(Command(true, mId, id, CommandType::Assault));
            mAction = Assaulting;
        }
    }
    else if(mAction == WithSquad) {
        if(distance(getPosition(), mLeader->getPosition()) > 30) {
            setVelocity(mDirection * dt.asSeconds() * (mSpeedBonus+mSpeedBase));
            seekTarget(mLeader->getPosition());
        }
        else { // Standby
            mVelocity = sf::Vector2f(0, 0);
            if(!prev) {
                mCommandQueue.push(Command(true, mId, mLeader->getId(), CommandType::InPosition));
                prev = true;
            }
        }
    }
    else if(mAction == Assaulting) {
        if(mTargeted == nullptr) {
            setDirection(1, 0);
            setVelocity(mDirection * dt.asSeconds() * (mSpeedBonus+mSpeedBase));
        }
        else mAction = Seeking;
    }
    /*
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
            setVelocity(mDirection * dt.asSeconds() * (mSpeedBonus+mSpeedBase));
        }
        else {
            mAction = Attacking;
            mTargeted->setAction(Attacking);
            mTargeted = mTargeted->getTarget();
        }
    }
     */
}

void Soldier::updateDefense(sf::Time dt) {
    if(mAction == None or isDestroyed()) return;
    else if(mAction == Override) {
        setVelocity(mDirection * dt.asSeconds() * (80.f+mSpeedBonus));
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
            setVelocity(mDirection * dt.asSeconds() * (mSpeedBonus+mSpeedBase));
            mEntityClock.restart();
            return;
        }
        /*
        if(distance(mOrigin, mTargeted->getPosition()) > 100) {
            mAction = Moving;
            return;
        }*/

        seekTarget();
        setVelocity(mDirection * dt.asSeconds() * (mSpeedBonus+mSpeedBase));
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

void Soldier::createTeam(int senderId) {
    if(mAction == Moving)
        mCommandQueue.push(Command(true, mId, senderId, CommandType::TeamAccept));
    else mCommandQueue.push(Command(true, mId, senderId, CommandType::TeamDeny));
}
/*
void Soldier::helpRequested(Soldier * ally) {
    //mTargeted = ally;
    mDirection = sf::Vector2f(0, 0);
    setVelocity(mDirection);
    isAvailable = false;
}

void Soldier::helpAlly(Soldier * ally) {
    mAction = Helping;
    mTargeted = ally;
    isAvailable = false;
}*/

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
        setVelocity(mDirection * dt.asSeconds() * (mSpeedBonus+mSpeedBase));
        return;
    }
    if(mTravelled < (float)mDistance) { // If there is still distance to travel
        // sf::Vector2f prev = getPosition();
        sf::Vector2f dpl = mDirection * dt.asSeconds() * (10.f+mSpeedBonus);
        setVelocity(dpl);
        mTravelled += distance(sf::Vector2f(0,0),mVelocity);
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

void Soldier::seekTarget(sf::Vector2f pos) {
    sf::Vector2f target = pos;
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

void Soldier::travel(){
    move(mVelocity);
}

void Soldier::changeBonus(Entity::Bonus bonus) {
    if(bonus == Entity::Castle){
        mGlow.setTextureRect(sf::IntRect(64,0,32,32));
        mBonus = bonus;
        mSpeedBonus = 15;
        mDamages = 25;
    }else if(bonus == Entity::Village and mBonus == Entity::None){
        mGlow.setTextureRect(sf::IntRect(32,0,32,32));
        mBonus = bonus;
        mSpeedBonus = 15;
        mDamages = 25;
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

void Soldier::setVelocity(sf::Vector2f dpl){
    mVelocity = dpl;
}

sf::Vector2f Soldier::getVelocity() {
    return mVelocity;
}

int Soldier::getId() {
    return mId;
}

void Soldier::setLeader(Soldier* leader) {
    mLeader = leader;
}

void Soldier::dropLeader() {
    mLeader = nullptr;
}

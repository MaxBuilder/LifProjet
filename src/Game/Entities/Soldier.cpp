//
// Created by 33771 on 23/02/2021.
//

#include "Soldier.hpp"

#include <iostream>

// Archer faire : ajouter les tables de données pour les entités
Soldier::Soldier(int id, EntityInfo::Team team, sf::Vector2i objectif, const TextureHolder& textures, const FontHolder& fonts, Pathfinding& Astar, CommandQueue& commandQueue)
: Entity(100,team, commandQueue)
, mId(id)
, mDisplayType(debug::life)
, mObjectif(objectif)
, mVelocity(0,0)
, mDirection(sf::Vector2f(0, 0))
, mOrigin(sf::Vector2f(0, 0))
, mSprite(textures.get(Textures::EntitySoldier))
, mGlow(textures.get(Textures::EntityGlow))
, mSpeedBase(15)
, mSpeedBonus(0)
, mBonus(EntityInfo::ID::None)
, mDamages(20)
, mTargeted(nullptr)
, mAction(Moving)
, mTravelled(0.f)
, mTargetInSight(0)
, mAllyInSight(0)
, mLeader(nullptr)
, nbRequested(0)
, nbResponse(0)
, prev(false)
, mSquadSize(0)
, nbInPlace(0)
, mPathfinding(Astar)
, usePathFinding(false)
{
    mBorder = 10;
    float blockSize = 20.f; // à modifier pour rendre dynamique

    // Fix origin and texture selection
    mGlow.setTextureRect(sf::IntRect(0,0,32,32));
    mTeam == EntityInfo::Team::Blue ? mSpriteRect= sf::IntRect(32,0,32,32) : mSpriteRect = sf::IntRect(32,32,32,32);
    mSprite.setTextureRect(mSpriteRect);
    mSprite.setScale(blockSize/mSprite.getLocalBounds().width,blockSize/mSprite.getLocalBounds().height);
    mGlow.setScale(blockSize/mSprite.getLocalBounds().width,blockSize/mSprite.getLocalBounds().height);
    setOrigin(blockSize/2.f,blockSize/2.f);
    mSpriteTime = sf::milliseconds(0);

    // Display init
    mDisplayID.setFont(fonts.get(Fonts::Main));
    mDisplayID.setFillColor(sf::Color::Black);
    mDisplayID.setCharacterSize(10u);
    std::string str = toString(mId);
    if (mTeam == EntityInfo::Team::Blue)
        str+=std::string(" Blue");
    else
        str+=std::string(" Red");
    mDisplayID.setString(str);
    mDisplayID.setPosition(getPosition()+sf::Vector2f(0,-20));
    centerOrigin(mDisplayID);

    mDisplayAction.setFont(fonts.get(Fonts::Main));
    mDisplayAction.setFillColor(sf::Color::Black);
    mDisplayAction.setCharacterSize(10u);
    mDisplayAction.setString("Moving");
    mDisplayAction.setPosition(getPosition()+sf::Vector2f(0,-20));
    centerOrigin(mDisplayID);

    backLife.setPosition(0,-11);
    backLife.setSize(sf::Vector2f(blockSize+1,5));
    backLife.setFillColor(sf::Color::Black);

    frontLife.setPosition(1,-10);
    frontLife.setSize(sf::Vector2f(blockSize-1,3));
    frontLife.setFillColor(sf::Color::Green);

}

void Soldier::drawCurrent(sf::RenderTarget &target, sf::RenderStates states) const {
    target.draw(mSprite, states);
    target.draw(mGlow,states);
    sf::Vertex line[] = {
            sf::Vertex(sf::Vector2f(getPosition()), sf::Color::Red),
            sf::Vertex(sf::Vector2f(getPosition() + mDirection * 10.f), sf::Color::Red)
    };
    target.draw(line, 2, sf::Lines);

    switch (mDisplayType) {
        case debug::id :
            target.draw(mDisplayID, states);
            break;
        case debug::action :
            target.draw(mDisplayAction, states);
            break;
        case debug::life :
            if(mHitPoints!=mMaxHintPoints and mHitPoints != 0){
                target.draw(backLife, states);
                target.draw(frontLife, states);
            }
            break;
        default :
            break;
    }
}

void Soldier::swithDebugDisplay(){
    if(mDisplayType == debug::id)
        mDisplayType = debug::action;
    else if(mDisplayType == debug::action)
        mDisplayType = debug::life;
    else
        mDisplayType = debug::id;
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

    mTeam == EntityInfo::Team::Blue ? updateDefense(dt) : updateAttack(dt);

    // update lifeDisplay
    float blockSize = 20.f;
    float xlife = float(mHitPoints)/float(mMaxHintPoints);
    float lifeLength = blockSize*xlife;

    frontLife.setSize(sf::Vector2f(lifeLength,3));

    if(xlife > 0.5f){
        frontLife.setFillColor(sf::Color::Green);
    }else if(xlife > 0.25f){
        frontLife.setFillColor(sf::Color::Yellow);
    }else{
        frontLife.setFillColor(sf::Color::Red);
    }

}

void Soldier::updateAttack(sf::Time dt) {

    if(mAction == None or isDestroyed()) return;
    else if(mAction == Override) {
        setVelocity(mDirection * dt.asSeconds() * (80.f+mSpeedBonus));
        return;
    }
    if(mAction == Moving) {
        if(mTargetInSight > 1 and mTargeted == nullptr) {
            // Lancement du signal et mise en groupe
            mCommandQueue.push(Command(true, mId, 9999, CommandType::MakeTeam));
            setAction(Calling);
            nbResponse = 0;
            nbRequested = 0;
        }

        if(mTargeted == nullptr) {
            if(mPath.empty()) {
                mAstarDuration.restart();
                mPathfinding.getPath(getPosition(), mObjectif, mPath, 2) ;
                std::cout << "Pathfinding Duration :" << mAstarDuration.getElapsedTime().asMicroseconds() << std::endl;
            }
            sf::Vector2f &target = mPath.back();
            if (distance(getPosition(), target) < 2 )
                mPath.pop_back();
            setDirection((target-getPosition())/norm(target-getPosition()));
            setVelocity(mDirection * dt.asSeconds() * (mSpeedBonus+mSpeedBase));
        }
        else setAction(Seeking);
    }
    else if(mAction == Seeking) {
        if(mTargeted == nullptr) {
            setAction(Moving);
            mSpeedBase = 15;
            //isAvailable = true;
            return;
        }

        if(distance(getPosition(), mTargeted->getPosition()) < mBorder+mTargeted->getBorder()) {
            setAction(Attacking);
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

        if(mTargeted == nullptr){
            setAction(Moving);
            return;
        }

        if(distance(getPosition(), mTargeted->getPosition()) >= mBorder+mTargeted->getBorder()+10) {
            setAction(Seeking);
            return;
        }

        attackTarget();

        if(mTargeted->isDestroyed()) {
            setAction(Moving);
        }
    }
    else if(mAction == Calling) {
        if(nbRequested != 0 and nbResponse == nbRequested) {
            if(mSquadSize == 0)
                setAction(Assaulting);
            else
                setAction(Leading);
            nbResponse = 0;
            nbRequested = 0;
        }
        else if(mTargeted != nullptr)
            setAction(Seeking);
        mVelocity = sf::Vector2f(0, 0);
    }
    else if(mAction == Leading) {
        if(nbInPlace == mSquadSize) {
            // Attack
            for (auto id : mSquadIds)
                mCommandQueue.push(Command(true, mId, id, CommandType::Assault));
            setAction(Assaulting);
            mSquadSize = 0;
            nbInPlace = 0;
            mSquadIds.clear();
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
        prev = false;
        if(mTargeted == nullptr) {
            setDirection(1, 0);
            setVelocity(mDirection * dt.asSeconds() * (mSpeedBonus+mSpeedBase));
        }
        else {
            setAction(Seeking);
            mLeader = nullptr;
        }
    }
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
            setAction(Seeking);
            mTravelled = 0;
        }
    }
    else if(mAction == Seeking) {
        if(mTargeted == nullptr) {
            setAction(Moving);
            mSpeedBase = 15;
            return;
        }

        if(distance(getPosition(), mTargeted->getPosition()) < mBorder+mTargeted->getBorder()) {
            setAction(Attacking);
            mDirection = sf::Vector2f(0, 0);
            setVelocity(mDirection * dt.asSeconds() * (mSpeedBonus+mSpeedBase));
            mEntityClock.restart();
            return;
        }

        seekTarget();
        setVelocity(mDirection * dt.asSeconds() * (mSpeedBonus+mSpeedBase));
    }
    else if(mAction == Attacking) {
        if(mTargeted == nullptr){
            setAction(Moving);
            return;
        }

        if(distance(getPosition(), mTargeted->getPosition()) >= mBorder+mTargeted->getBorder()+10) {
            setAction(Seeking);
            return;
        }

        attackTarget();

        // Add leave condition here (flee or stay)
        if(mTargeted->isDestroyed()) {
            setAction(Moving);
        }
    }
}

void Soldier::createTeam(int senderId) {
    if(mAction == Moving)
        mCommandQueue.push(Command(true, mId, senderId, CommandType::TeamAccept));
    else mCommandQueue.push(Command(true, mId, senderId, CommandType::TeamDeny));
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

void Soldier::seekTarget(sf::Vector2f pos) {
    sf::Vector2f target;
    if (pos == sf::Vector2f(-1,-1))
        target = mTargeted->getPosition();
    else
        target = pos;
    if(usePathFinding){
        if(mPath.empty()) {
            mAstarDuration.restart();
            mPathfinding.getPath(getPosition(), target/20.f, mPath, 2) ;
            std::cout << "Pathfinding Duration :" << mAstarDuration.getElapsedTime().asMicroseconds() << std::endl;
        }
        target = mPath.back();
        if (distance(getPosition(), target) < 2 )
            mPath.pop_back();
        setDirection((target-getPosition())/norm(target-getPosition()));
    }
    else{
        target = target - getPosition();
        mDirection = target / norm(target);
    }
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
    mPath.clear();
    usePathFinding = false;
    std::string name[10] = {"None","Moving","Seeking","Fleeing","Attacking","Calling","Leading","WithSquad","Assaulting","controlling"};
    std::string str = mTeam ==EntityInfo::Team::Blue ? "Blue" : "Red";
    std::cout<<"id : "<<mId<<str<<" action :"<<name[mAction]<<" -> "<<name[act]<<std::endl;
    mAction = act;
    mDisplayAction.setString(name[mAction]);
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

void Soldier::setTarget(Entity* target) {
    mTargeted = target;
}

void Soldier::dropTarget() {
    mTargeted = nullptr;
}

Entity* Soldier::getTarget() {
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

void Soldier::changeBonus(EntityInfo::ID bonus) {
    if(bonus == EntityInfo::Castle){
        mGlow.setTextureRect(sf::IntRect(64,0,32,32));
        mBonus = bonus;
        mSpeedBonus = 15;
        mDamages = 25;
    }
    else if(bonus == EntityInfo::Village and mBonus == EntityInfo::None){
        mGlow.setTextureRect(sf::IntRect(32,0,32,32));
        mBonus = bonus;
        mSpeedBonus = 15;
        mDamages = 25;
    }
    else if(bonus == EntityInfo::None){
        mGlow.setTextureRect(sf::IntRect(0,0,32,32));
        mBonus = bonus;
        mSpeedBonus = 0;
        mDamages = 20;
    }
}
EntityInfo::ID Soldier::getBonus(){
    return mBonus;
}

void Soldier::setVelocity(sf::Vector2f dpl){
    mVelocity = dpl;
}

sf::Vector2f Soldier::getVelocity() {
    return mVelocity;
}

int Soldier::getId() const {
    return mId;
}

void Soldier::setLeader(Soldier* leader) {
    mLeader = leader;
}

void Soldier::dropLeader() {
    mLeader = nullptr;
}

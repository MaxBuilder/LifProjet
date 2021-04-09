//
// Created by 33771 on 23/02/2021.
//

#include "Soldier.hpp"

#include <iostream>

// Archer faire : ajouter les tables de données pour les entités
Soldier::Soldier(int id, EntityInfo::ID soldierType, EntityInfo::Team team, sf::Vector2i objectif, const TextureHolder& textures, const FontHolder& fonts, Pathfinding& Astar, CommandQueue& commandQueue)
: Entity(100,team, commandQueue)
, mId(id)
, mSoldierType(soldierType)
, mDisplayType(debug::life)
, mObjectif(objectif)
, mVelocity(0,0)
, mDirection(sf::Vector2f(0, 0))
, mOrigin(sf::Vector2f(0, 0))
, mSprite()
, mGlow(textures.get(Textures::EntityGlow))
, mSpeedBase(15)
, mSpeedBonus(0)
, mBonus(EntityInfo::ID::None)
, mDamages(20)
, mDistance(0)
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
, sendAck(false)
, mEntityTime()
{
    //std::cout << objectif.x << " " << objectif.y << std::endl;

    if (mSoldierType == EntityInfo::Knight)
        team == EntityInfo::Blue ? mSprite.setTexture(textures.get(Textures::EntityKnightBlue)):
                                   mSprite.setTexture(textures.get(Textures::EntityKnightRed ));
    else if (mSoldierType == EntityInfo::Archer)
        team == EntityInfo::Blue ? mSprite.setTexture(textures.get(Textures::EntityArcherBlue)):
                                   mSprite.setTexture(textures.get(Textures::EntityArcherRed ));
    else if (mSoldierType == EntityInfo::Tank)
        team == EntityInfo::Blue ? mSprite.setTexture(textures.get(Textures::EntityPaladinBlue)):
        mSprite.setTexture(textures.get(Textures::EntityPaladinRed));

    mBorder = 10;
    float blockSize = 20.f; // à modifier pour rendre dynamique

    // Fix origin and texture selection
    mGlow.setTextureRect(sf::IntRect(0,0,32,32));
    mSpriteRect = sf::IntRect(0,0,64,32);
    mSprite.setTextureRect(mSpriteRect);
    mSprite.setScale(blockSize*2/mSprite.getLocalBounds().width,blockSize/mSprite.getLocalBounds().height);
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

void Soldier::switchDebugDisplay() {
    if(mDisplayType == debug::id)
        mDisplayType = debug::action;
    else if(mDisplayType == debug::action)
        mDisplayType = debug::life;
    else
        mDisplayType = debug::id;
}

void Soldier::updateCurrent(sf::Time dt) {

    // Update mecanics
    mTeam == EntityInfo::Team::Blue ? updateDefense(dt) : updateAttack(dt);

    //update sprites
    updateSprite(dt);

    // update lifeDisplay
    float blockSize = 20.f;
    float xlife = float(mHitPoints)/float(mMaxHintPoints);
    float lifeLength = blockSize * xlife;

    frontLife.setSize(sf::Vector2f(lifeLength,3));

    if(xlife > 0.5f)
        frontLife.setFillColor(sf::Color::Green);
    else if(xlife > 0.25f)
        frontLife.setFillColor(sf::Color::Yellow);
    else frontLife.setFillColor(sf::Color::Red);

}

void Soldier::updateSprite(sf::Time dt){

    mSpriteTime = sf::milliseconds(dt.asMilliseconds()+mSpriteTime.asMilliseconds());
    if (mSpriteTime.asMilliseconds() > 130 ){

        if(mSpriteRect.top < 96)
            mSpriteRect.left += 64;
        else
            mSpriteRect.left -= 64;

        if(isDestroyed()){
            if(mSpriteRect.top < 96) {
                if(mSpriteRect.top != 64) {
                    mSpriteRect.top = 64;
                    mSpriteRect.left = 0;
                }
                if(mSpriteRect.left > 64*5)
                    mSpriteRect.left = 64*5;
            }
            else {
                if(mSpriteRect.top != 32*5){
                    mSpriteRect.top = 32*5;
                    mSpriteRect.left = 64*5;
                }
                if(mSpriteRect.left <= 0)
                    mSpriteRect.left = 0;
            }

        }
        else if(mAction == Attacking){
            float x = getPosition().x - mTargeted->getPosition().x;
            if (x < 0 and (mSpriteRect.left > 64*5 or mSpriteRect.left < 0 or mSpriteRect.top != 32) ) {
                mSpriteRect.left = 0;
                mSpriteRect.top = 32;
            }
            else if(x > 0 and (mSpriteRect.left > 64*5 or mSpriteRect.left < 0 or mSpriteRect.top != 32*4)) {
                mSpriteRect.left = 64*5;
                mSpriteRect.top = 32*4;
            }

        }
        else if (mDirection.x > 0 and (mSpriteRect.left > 64*5 or mSpriteRect.left < 0 or mSpriteRect.top != 0)) {
            mSpriteRect.left = 0;
            mSpriteRect.top = 0;

        }else if (mDirection.x < 0 and  (mSpriteRect.left > 64*5 or mSpriteRect.left < 0 or mSpriteRect.top != 32*3) ) {
            mSpriteRect.left = 64 * 5;
            mSpriteRect.top = 32 * 3;
        }else if(mSpriteRect.left > 64*5){
            mSpriteRect.left = 0;
        }else if(mSpriteRect.left < 0){
            mSpriteRect.left = 64*5;
        }

        if(mSpriteRect.top < 96)
            mSprite.setPosition(0,0);
        else
            mSprite.setPosition(-20,0);

        mSpriteTime = sf::milliseconds(0);
        mSprite.setTextureRect(mSpriteRect);
    }


    mEntityTime += dt;

}

void Soldier::updateAttack(sf::Time dt) {

    if(mAction == None or isDestroyed())
        return;
    if(mAction == Moving) {
        if(mTargetInSight > 1 and mTargeted == nullptr) {
            // Lancement du signal et mise en groupe
            mCommandQueue.push(Command(mTeam, mId, 9999, CommandType::MakeTeam));
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
            mEntityTime = sf::seconds(0);
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

        if(distance(getPosition(), mTargeted->getPosition()) >= mBorder+mTargeted->getBorder() + 10) {
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
                mCommandQueue.push(Command(mTeam, mId, id, CommandType::Assault));
            setAction(Assaulting);
            mSquadSize = 0;
            nbInPlace = 0;
            mSquadIds.clear();
        }
    }
    else if(mAction == WithSquad) {
        if(distance(getPosition(), mLeader->getPosition()) > 30) {
            setVelocity(mDirection * dt.asSeconds() * (mSpeedBonus + mSpeedBase));
            seekTarget(mLeader->getPosition());
        }
        else { // Standby
            mVelocity = sf::Vector2f(0, 0);
            if(!prev) {
                mCommandQueue.push(Command(mTeam, mId, mLeader->getId(), CommandType::InPosition));
                prev = true;
            }
        }
    }
    else if(mAction == Assaulting) {
        prev = false;
        sendAck = false;
        if(mTargeted == nullptr) {
            setDirection(1, 0);
            setVelocity(mDirection * dt.asSeconds() * (mSpeedBonus + mSpeedBase));
        }
        else {
            setAction(Seeking);
            mLeader = nullptr;
        }
    }
}

void Soldier::updateDefense(sf::Time dt) {
    if(mAction == None or isDestroyed())
        return;
    if(mAction == Moving) {
        if(mTravelled == 0)
            setDirection(randomDirection());

        if(mTargetInSight > 1 and mTargeted == nullptr) {
            // Lancement du signal et mise en groupe
            mCommandQueue.push(Command(mTeam, mId, 9999, CommandType::MakeTeam));
            setAction(Calling);
            nbResponse = 0;
            nbRequested = 0;
        }

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
            setVelocity(mDirection * dt.asSeconds() * (mSpeedBonus + mSpeedBase));
            mEntityTime = sf::seconds(0);
            return;
        }

        seekTarget();
        setVelocity(mDirection * dt.asSeconds() * (mSpeedBonus + mSpeedBase));
    }
    else if(mAction == Attacking) {
        if(mTargeted == nullptr){
            setAction(Moving);
            return;
        }

        if(distance(getPosition(), mTargeted->getPosition()) >= mBorder+mTargeted->getBorder() + 10) {
            setAction(Seeking);
            return;
        }

        attackTarget();

        // Add leave condition here (flee or stay)
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
                mCommandQueue.push(Command(mTeam, mId, id, CommandType::Assault));
            setAction(Assaulting);
            mSquadSize = 0;
            nbInPlace = 0;
            mSquadIds.clear();
        }
    }
    else if(mAction == WithSquad) {
        if(distance(getPosition(), mLeader->getPosition()) > 30) {
            setVelocity(mDirection * dt.asSeconds() * (mSpeedBonus + mSpeedBase));
            seekTarget(mLeader->getPosition());
        }
        else { // Standby
            mVelocity = sf::Vector2f(0, 0);
            if(!prev) {
                mCommandQueue.push(Command(mTeam, mId, mLeader->getId(), CommandType::InPosition));
                prev = true;
            }
        }
    }
    else if(mAction == Assaulting) {
        prev = false;
        sendAck = false;

        if(mLeader == nullptr) {
            setAction(Moving);
            return;
        }
        if(mTargeted == nullptr) {
            setDirection(-1, 0);
            setVelocity(mDirection * dt.asSeconds() * (mSpeedBonus + mSpeedBase));
        }
        else {
            setAction(Seeking);
            mLeader = nullptr;
        }
    }
    else if(mAction == DefendingCastle) {
        usePathFinding = true;
        seekTarget(mObjectif * 20.f);
        setVelocity(mDirection * dt.asSeconds() * (mSpeedBonus + mSpeedBase));

        if(mTargeted != nullptr) {
            setAction(Attacking);
            mDirection = sf::Vector2f(0, 0);
            setVelocity(mDirection * dt.asSeconds() * (mSpeedBonus + mSpeedBase));
            mEntityTime = sf::seconds(0);
            return;
        }
    }
}

void Soldier::createTeam(int senderId) {
    if(mAction == Moving and !sendAck) {
        mCommandQueue.push(Command(mTeam, mId, senderId, CommandType::TeamAccept));
        sendAck = true;
    }
    else mCommandQueue.push(Command(mTeam, mId, senderId, CommandType::TeamDeny));
}

void Soldier::attackTarget() {
    if(mEntityTime.asSeconds() > 1) {
        mTargeted->damage(mDamages);
        mEntityTime = sf::seconds(0);
    }
    if(mTargeted->isDestroyed())
        mTargeted->remove();
}

void Soldier::roam(sf::Time dt) {
    if(distance(getPosition(), mOrigin) > 50) { // If too far from origin, go back
        mDistance = 10;
        mTravelled = 0;
        setDirection(mOrigin - getPosition());
        setVelocity(mDirection * dt.asSeconds() * (mSpeedBonus + mSpeedBase));
        return;
    }
    if(mTravelled < (float)mDistance) { // If there is still distance to travel
        // sf::Vector2f prev = getPosition();
        sf::Vector2f dpl = mDirection * dt.asSeconds() * (10.f + mSpeedBonus);
        setVelocity(dpl);
        mTravelled += distance(sf::Vector2f(0,0), mVelocity);
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
    if(usePathFinding) {
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
    else {
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
    std::string name[10] = {"None", "Moving", "Seeking", "Attacking", "Calling", "Leading", "WithSquad", "Assaulting", "DefendingCastle"};
    std::string str = mTeam == EntityInfo::Team::Blue ? "Blue" : "Red";
    std::cout << "id : " << mId << str << " action :" << name[mAction] << " -> " << name[act] << std::endl;
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

void Soldier::setTarget(Entity* target) {
    mTargeted = target;
}

void Soldier::init() {
    mOrigin = getPosition();
}

void Soldier::travel(){
    move(mVelocity);
}

void Soldier::changeBonus(EntityInfo::ID bonus) {
    if(bonus == EntityInfo::Castle) {
        mGlow.setTextureRect(sf::IntRect(64,0,32,32));
        mBonus = bonus;
        mSpeedBonus = 15;
        mDamages = 25;
    }
    else if(bonus == EntityInfo::Village and mBonus == EntityInfo::None) {
        mGlow.setTextureRect(sf::IntRect(32,0,32,32));
        mBonus = bonus;
        mSpeedBonus = 15;
        mDamages = 25;
    }
    else if(bonus == EntityInfo::None) {
        mGlow.setTextureRect(sf::IntRect(0,0,32,32));
        mBonus = bonus;
        mSpeedBonus = 0;
        mDamages = 20;
    }
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

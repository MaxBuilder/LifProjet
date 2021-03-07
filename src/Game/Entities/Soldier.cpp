//
// Created by 33771 on 23/02/2021.
//

#include "Soldier.hpp"

#include <iostream>

// A faire : ajouter les tables de données pour les entités
Soldier::Soldier(Team team, const TextureHolder& textures, const FontHolder& fonts)
: Entity(100)
, mDirection(sf::Vector2i(0, 0))
, mSprite(textures.get(Textures::EntitySoldier))
, mTeam(team)
, mSpeed(15)
, mTargeted(nullptr)
, mAction(Move)
, mTravelled(0.f)
{
    mTeam == BlueTeam ? mSprite.setTextureRect(sf::IntRect(20, 0, 20, 20)) : mSprite.setTextureRect(sf::IntRect(40, 0, 20, 20));
}

void Soldier::drawCurrent(sf::RenderTarget &target, sf::RenderStates states) const {
    target.draw(mSprite, states);
    sf::Vertex line[] = {
            sf::Vertex(sf::Vector2f(getPosition() + sf::Vector2f(10, 10)), sf::Color::Red),
            sf::Vertex(sf::Vector2f(getPosition() + mDirection * 10.f) + sf::Vector2f(10, 10), sf::Color::Red)
    };
    target.draw(line, 2, sf::Lines);
}

void Soldier::updateCurrent(sf::Time dt) {
    mTeam == BlueTeam ? updateDefense(dt) : updateAttack(dt);
}

void Soldier::updateAttack(sf::Time dt) {
    if(mAction == None or isDestroyed()) return;
    else if(mAction == Override) {
        move(mDirection * dt.asSeconds() * 80.f);
        return;
    }
    if(mAction == Move) {
        if(mTargeted == nullptr) {
            setDirection(sf::Vector2f(1, 0));
            move(mDirection * dt.asSeconds() * mSpeed);
        }
        else mAction = Seek;
    }
    else if(mAction == Seek) {
        if(mTargeted == nullptr) {
            mAction = Move;
            mSpeed = 15;
            return;
        }

        if(distance(getPosition(), mTargeted->getPosition()) < 20) {
            mAction = Attack;
            return;
        }

        seekTarget();
        move(mDirection * dt.asSeconds() * mSpeed);
    }
    else if(mAction == Attack) {
        mDirection = sf::Vector2f(0, 0);
        mTargeted->remove();

        if(mTargeted->isDestroyed())
            mAction = Move;
    }
}

void Soldier::updateDefense(sf::Time dt) {
    if(mAction == None or isDestroyed()) return;
    else if(mAction == Override) {
        move(mDirection * dt.asSeconds() * 80.f);
        return;
    }
    if(mAction == Move) {
        if(mTravelled == 0)
            setDirection(randomDirection());

        if(mTargeted == nullptr) {
            roam(dt);
        }
        else {
            mAction = Seek;
            mTravelled = 0;
        }
    }
    else if(mAction == Seek) {
        if(mTargeted == nullptr) {
            mAction = Move;
            mSpeed = 15;
            return;
        }

        if(distance(getPosition(), mTargeted->getPosition()) < 20) {
            mAction = Attack;
            return;
        }

        if(distance(mOrigin, mTargeted->getPosition()) < 100) {
            mAction = Move;
            return;
        }

        seekTarget();
        move(mDirection * dt.asSeconds() * mSpeed);
    }
    else if(mAction == Attack) {
        mDirection = sf::Vector2f(0, 0);
        mTargeted->remove();

        if(mTargeted->isDestroyed()) {
            mAction = Move;
        }
    }
}

void Soldier::roam(sf::Time dt) {
    if(mTravelled < (float)mDistance) {
        sf::Vector2f prev = getPosition();
        sf::Vector2f toMove = mDirection * dt.asSeconds() * 10.f;
        move(toMove);
        mTravelled += distance(prev, getPosition());
        return;
    }
    mTravelled = 0;
    setDirection(randomDirection());
}

sf::Vector2f Soldier::randomDirection() {
    mDistance = Random::Generate(10.f, 50.f);
    return sf::Vector2f(Random::Generate(-10.f, 10.f), Random::Generate(-10.f, 10.f));
}

void Soldier::seekTarget() {
    sf::Vector2f target = mTargeted->getPosition();
    target = target - getPosition();
    mDirection = target / norm(target);
    mSpeed = 30;
}

void Soldier::fleeTarget() {
    if(mTargeted != nullptr) {
        sf::Vector2f target = mTargeted->getPosition();
        target = getPosition() - target;
        mDirection = target / norm(target);
        mSpeed = 30;
    }
    else mSpeed = 15;
}

Soldier::Team Soldier::getTeam() {
    return mTeam;
}

Soldier::Action Soldier::getAction() {
    return mAction;
}

void Soldier::setAction(Action act) {
    mAction = act;
}

void Soldier::remove() {
    Entity::remove();
    mSprite.setTextureRect(sf::IntRect(0, 0, 20, 20));
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
    return mSpeed;
}

void Soldier::setSpeed(float speed) {
    mSpeed = speed;
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
    //mOrigin = getPosition();
}

void Soldier::resetTravelledDistance() {
    mTravelled = 0;
}
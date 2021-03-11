//
// Created by 33771 on 23/02/2021.
//

#include "Soldier.hpp"

#include <iostream>

// A faire : ajouter les tables de données pour les entités
Soldier::Soldier(Team team, const TextureHolder& textures, const FontHolder& fonts, bool big)
: Entity(100)
, mDirection(sf::Vector2f(0, 0))
, mOrigin(sf::Vector2f(0, 0))
, mSprite(textures.get(Textures::EntitySoldier))
, mTeam(team)
, mSpeed(15)
, mTargeted(nullptr)
, mAction(Moving)
, mTravelled(0.f)
, isBigBitch(big)
, isAvailable(true)
{
    // Fix origin and texture selection
    setOrigin(getPosition().x + 10, getPosition().y + 10);
    mTeam == BlueTeam ?
    (isBigBitch ? mSprite.setTextureRect(sf::IntRect(60, 0, 20, 20)) : mSprite.setTextureRect(sf::IntRect(20, 0, 20, 20)))
    :
    (isBigBitch ? mSprite.setTextureRect(sf::IntRect(80, 0, 0, 0)) : mSprite.setTextureRect(sf::IntRect(40, 0, 20, 20)));

    if(isBigBitch) heal(100); // HP to 200 for big entities

    mLife.setFont(fonts.get(Fonts::Main));
    mLife.setFillColor(sf::Color::Black);
    mLife.setCharacterSize(10u);
}

void Soldier::drawCurrent(sf::RenderTarget &target, sf::RenderStates states) const {
    target.draw(mSprite, states);
    sf::Vertex line[] = {
            sf::Vertex(sf::Vector2f(getPosition()), sf::Color::Red),
            sf::Vertex(sf::Vector2f(getPosition() + mDirection * 10.f), sf::Color::Red)
    };
    target.draw(line, 2, sf::Lines);
    target.draw(mLife);
}

void Soldier::updateCurrent(sf::Time dt) {
    mTeam == BlueTeam ? updateDefense(dt) : updateAttack(dt);
    mLife.setString(std::to_string(getHitPoints()));
    mLife.setPosition(getPosition());
    centerOrigin(mLife);
}

void Soldier::updateAttack(sf::Time dt) {
    if(mAction == None or isDestroyed()) return;
    else if(mAction == Override) {
        move(mDirection * dt.asSeconds() * 80.f);
        return;
    }
    if(mAction == Moving) {
        if(mTargeted == nullptr) {
            setDirection(sf::Vector2f(1, 0));
            move(mDirection * dt.asSeconds() * mSpeed);
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
            mSpeed = 15;
            isAvailable = true;
            return;
        }

        if(distance(getPosition(), mTargeted->getPosition()) < 20) {
            mAction = Attacking;
            mDirection = sf::Vector2f(0, 0);
            mEntityClock.restart();
            mSpeed = 15;
            return;
        }

        seekTarget();
        move(mDirection * dt.asSeconds() * mSpeed);
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
            move(mDirection * dt.asSeconds() * mSpeed);
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
        move(mDirection * dt.asSeconds() * 80.f);
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
            mSpeed = 15;
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
        move(mDirection * dt.asSeconds() * mSpeed);
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
        mTargeted->damage(20);
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
        move(mDirection * dt.asSeconds() * 10.f);
        return;
    }
    if(mTravelled < (float)mDistance) { // If there is still distance to travel
        sf::Vector2f prev = getPosition();
        sf::Vector2f toMove = mDirection * dt.asSeconds() * 10.f;
        move(toMove);
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
    mOrigin = getPosition();
}

void Soldier::resetTravelledDistance() {
    mTravelled = 0;
}

sf::Vector2f Soldier::getOrigin() {
    return mOrigin;
}
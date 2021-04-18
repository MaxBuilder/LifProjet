//
// Created by 33771 on 23/02/2021.
//

#include "Entity.hpp"

Entity::Entity(EntityInfo::ID type, EntityInfo::Team team, int hitPoints, CommandQueue &commandQueue)
: mTeam(team)
, mType(type)
, mBorder(0)
, down(false)
, mCommandQueue(commandQueue) {
    setHealth(hitPoints);
}

void Entity::heal(int points) {
    mHitPoints += points;
    if(mHitPoints > mMaxHintPoints)
        mHitPoints = mMaxHintPoints;
}

void Entity::damage(int points) {
    mHitPoints -= points;
    if (mHitPoints <= 0)
        mCommandQueue.push(Command(mTeam,0,0,CommandType::Dead));
}

void Entity::destroy() {
    mHitPoints = 0;
}

void Entity::remove() {
    destroy();
}

bool Entity::isDestroyed() const {
    return mHitPoints <= 0;
}

float Entity::getBorder() const{
    return mBorder;
}

EntityInfo::Team Entity::getTeam() {
    return mTeam;
}

void Entity::updateCurrent(sf::Time dt) {

}

EntityInfo::ID Entity::getType() const {
    return mType;
}

void Entity::setHealth(int health) {
    mHitPoints = health;
    mMaxHintPoints = health;
}

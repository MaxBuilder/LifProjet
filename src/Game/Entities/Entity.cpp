//
// Created by 33771 on 23/02/2021.
//

#include "Entity.hpp"

#include <iostream>

Entity::Entity(int hitPoints, Team team)
: mHitPoints(hitPoints),
  mTeam(team),
  mBorder(0),
  down(false)
{}

void Entity::heal(int points) {
    mHitPoints += points;
}

void Entity::damage(int points) {
    mHitPoints -= points;
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

int Entity::getHitPoints() const {
    return mHitPoints;
}

float Entity::getBorder() const{
    return mBorder;
}

Entity::Team Entity::getTeam() {
    return mTeam;
}

void Entity::updateCurrent(sf::Time dt) {

}
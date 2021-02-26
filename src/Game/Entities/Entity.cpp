//
// Created by 33771 on 23/02/2021.
//

#include "Entity.hpp"

#include <iostream>

Entity::Entity(int hitPoints)
: mVelocity(sf::Vector2i(0, 0))
, mHitPoints(hitPoints)
{}

void Entity::setVelocity(sf::Vector2f velocity) {
    mVelocity = velocity;
}

void Entity::setVelocity(float vx, float vy) {
    mVelocity.x = vx;
    mVelocity.y = vy;
}

void Entity::accelerate(sf::Vector2f velocity) {
    mVelocity += velocity;
}

void Entity::accelerate(float vx, float vy) {
    mVelocity.x += vx;
    mVelocity.y += vy;
}

sf::Vector2f Entity::getVelocity() const {
    return mVelocity;
}

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
    return mHitPoints > 0;
}

int Entity::getHitPoints() const {
    return mHitPoints;
}

void Entity::updateCurrent(sf::Time dt) {
    move(mVelocity * dt.asSeconds());
}
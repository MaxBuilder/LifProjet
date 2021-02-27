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
, mSpeed(10)
{


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
    move(mDirection * dt.asSeconds() * mSpeed);
}

Soldier::Team Soldier::getTeam() {
    return mTeam;
}
/*
sf::FloatRect Soldier::getBoundingBox() {
    return getTransform().transformRect(mSprite.getGlobalBounds());
}
*/
void Soldier::remove() {
    Entity::remove();
}

void Soldier::setDirection(sf::Vector2f velocity) {
    mDirection = velocity;
}

void Soldier::setDirection(float vx, float vy) {
    mDirection.x = vx;
    mDirection.y = vy;
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

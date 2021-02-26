//
// Created by 33771 on 23/02/2021.
//

#include "Soldier.hpp"

#include <iostream>

// A faire : ajouter les tables de données pour les entités
Soldier::Soldier(Team team, const TextureHolder& textures, const FontHolder& fonts)
: Entity(100)
, mTeam(team) {
    setVelocity(10, 10);

    mTemp.setRadius(10);
    mTemp.setOutlineThickness(1);
    mTemp.setFillColor(sf::Color(0, 0, 0, 0));
    mTemp.setOutlineColor(sf::Color::White);
    mTemp.setPosition(100, 100);

}

void Soldier::drawCurrent(sf::RenderTarget &target, sf::RenderStates states) const {
    target.draw(mTemp, states);
}

void Soldier::updateCurrent(sf::Time dt) {

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

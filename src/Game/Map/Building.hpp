//
// Created by 33771 on 18/02/2021.
//

#ifndef LIFPROJET_BUILDING_HPP
#define LIFPROJET_BUILDING_HPP

#include "../../Game/Resources/ResourceIdentifiers.hpp"
#include "SFML/Graphics/Sprite.hpp"

class Building {
public:
    Building(Buildings::ID ID, sf::IntRect position);

    Buildings::ID getID() const;
    sf::IntRect getPosition() const;
    void setPosition(sf::IntRect rect);

private:
    Buildings::ID mId;

    sf::IntRect mPosition;
};


#endif //LIFPROJET_BUILDING_HPP

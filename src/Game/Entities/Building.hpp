//
// Created by thibaut on 12/03/2021.
//

#ifndef LIFPROJET_BUILDING_HPP
#define LIFPROJET_BUILDING_HPP

#include "../../Game/Resources/ResourceIdentifiers.hpp"
#include <SFML/Graphics/RenderTarget.hpp>
#include "SFML/Graphics/CircleShape.hpp"

#include "Entity.hpp"

class Building : public Entity {
public:
    Building(Buildings::ID ID, sf::IntRect position);

    float getRange() const;
    Entity::Bonus getBonusFlag() const;

private:

    void drawCurrent(sf::RenderTarget& target, sf::RenderStates states) const override;

    float mRange;
    Entity::Bonus mBonusFlag;
    sf::CircleShape zone;
    sf::IntRect mPosition;

};


#endif //LIFPROJET_BUILDING_HPP

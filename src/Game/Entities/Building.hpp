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
    Building(Buildings::ID ID, sf::IntRect position,  CommandQueue& commandQueue);

    float getRange() const;

    Entity::Bonus getBonusFlag() const;
    sf::Vector2i getMapId() const;
    sf::Vector2i getOnMapPosition() const;
    sf::Vector2i getOnMapSize() const;

private:

    void drawCurrent(sf::RenderTarget& target, sf::RenderStates states) const override;
    void updateCurrent();

    float mRange;
    Entity::Bonus mBonusFlag;
    sf::CircleShape zone;
    sf::IntRect mPosition;
    sf::Vector2i mTextureId;
};


#endif //LIFPROJET_BUILDING_HPP

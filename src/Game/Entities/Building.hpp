//
// Created by thibaut on 12/03/2021.
//

#ifndef LIFPROJET_BUILDING_HPP
#define LIFPROJET_BUILDING_HPP

#include <SFML/Graphics/RenderTarget.hpp>
#include "SFML/Graphics/CircleShape.hpp"


#include "Entity.hpp"

class Building : public Entity {
public:
    Building(EntityInfo::ID ID,EntityInfo::Team team, sf::IntRect position,  CommandQueue& commandQueue);

    float getRange() const;

    EntityInfo::ID getBonusFlag() const;
    sf::Vector2i getMapId() const;
    sf::Vector2i getOnMapPosition() const;
    sf::Vector2i getOnMapSize() const;

private:

    void drawCurrent(sf::RenderTarget& target, sf::RenderStates states) const override;
    void updateCurrent();

    float mRange;
    EntityInfo::ID mBonusFlag;
    sf::CircleShape mZone;
    sf::IntRect mPosition;
    sf::Vector2i mTextureId;
};


#endif //LIFPROJET_BUILDING_HPP

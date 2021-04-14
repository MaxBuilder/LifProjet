//
// Created by thibaut on 12/03/2021.
//

#ifndef LIFPROJET_BUILDING_HPP
#define LIFPROJET_BUILDING_HPP

#include "Entity.hpp"

#include <SFML/Graphics/RenderTarget.hpp>
#include "SFML/Graphics/CircleShape.hpp"
#include "SFML/Graphics/RectangleShape.hpp"

/**
 * @class Building
 * @brief Stores all the information of the building entities
 */
class Building : public Entity {
public:
    /**
     * @brief Parameter constructor
     * @param ID Type of the building
     * @param team Team
     * @param position Position of the building
     * @param commandQueue Reference to the command queue
     */
    Building(EntityInfo::ID ID,EntityInfo::Team team, sf::Vector2f position,  CommandQueue& commandQueue);

    /**
     * @brief Returns range of the building
     * @return Range
     */
    float getRange() const;

    /**
     * @brief Returns the type of bonus
     * @return Type of the bonus
     */
    EntityInfo::ID getBonusFlag() const;
    /**
     * @brief Returns the Id
     * @return Id
     */
    sf::Vector2i getMapId() const;
    /**
     * @brief Returns the position on the map
     * @return Position on the map
     */
    sf::Vector2i getOnMapPosition() const;
    /**
     * @brief Returns the size on the map
     * @return
     */
    sf::Vector2i getOnMapSize() const;

private:

    /**
         * @brief Draws the object to the target
         * @param target Target to render the object to
         * @param states Informations on how to render the object
         */
    void drawCurrent(sf::RenderTarget& target, sf::RenderStates states) const override;
    /**
     * @brief Updates the building
     * @param dt Time interval since last update
     */
    void updateCurrent(sf::Time dt) override;

    float mRange;
    sf::RectangleShape backLife,frontLife;
    EntityInfo::ID mBonusFlag;
    sf::CircleShape mZone;
    sf::IntRect mPosition;
    sf::Vector2i mTextureId;
    bool previouslyHit;
    sf::Time mBuildingTime;
    int prevHealth;
};


#endif //LIFPROJET_BUILDING_HPP

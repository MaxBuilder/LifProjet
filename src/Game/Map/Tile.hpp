//
// Created by thibaut on 01/02/2021.
//

#ifndef LIFPROJET_TILE_HPP
#define LIFPROJET_TILE_HPP

#include "../../Game/Resources/ResourceIdentifiers.hpp"
#include "EntityInfo.hpp"
#include "SFML/Graphics/Sprite.hpp"

/**
 * @class Tile
 * @brief Stores all the information about a Tile
 */
class Tile {
public:
    /**
    * @brief Constructor initialise the Tile with default values
    */
    Tile();

    /**
    * @brief Get the texture id of the ground ( which is also the coordinate of sprite in the texture)
    * @return Return a vector of coordinate
    */
    sf::Vector2i getGround() const;
    /**
    * @brief Get the texture id of the top ( build, trees, stone, etc )
    * @return Return a vector of coordinate
    */
    sf::Vector2i getTop() const;

    /**
    * @brief Return true if the Tile have a top ( build, tree, brigde ..)
    * @return A boolean
    */
    bool haveTop() const;
    /**
    * @brief Get the speed of the ground or the top of the Tile
    * @return A float
    */
    float getMoveSpeed() const;
    /**
    * @brief Get the rotation of the Tile
    * @return A float
    */
    float getRotation() const;
    /**
    * @brief Return true if the Tile is crossable by an entity (use by pathfinding)
    * @return A boolean
    */
    bool isCrossable() const;
    /**
    * @brief Return true if the Tile is crossable by an entity ( use by entities )
    * @param The team of the entity which want to cross the Tile
    * @return A boolean
    */
    bool isCrossable(EntityInfo::Team team) const;

    /**
    * @brief Set new Tile informations according the given id
    * @param Id of the new texture for the Tile
    * @param Rotation of the Tile
    */
    void paint(const sf::Vector2i &id,const float &rotation);
    /**
    * @brief Set the rotation of the Tile
    * @param The new roation of the Tile
    */
    void setRotation(const float &rotation);

private:
    sf::Vector2i ground, top;

    float rotate;
    float moveSpeed,topMoveSpeed;
    bool crossable, topCrossable, mHaveTop;

};

#endif //LIFPROJET_TILE_HPP

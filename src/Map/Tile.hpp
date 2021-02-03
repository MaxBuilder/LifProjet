//
// Created by thibaut on 01/02/2021.
//

#ifndef LIFPROJET_TILE_HPP
#define LIFPROJET_TILE_HPP

#include "../Resources/ResourceIdentifiers.hpp"
#include "SFML/Graphics/Sprite.hpp"

class Tile {
public:
    Tile();

    sf::Sprite& getSprite();
    sf::Sprite getConstSprite() const;

    Textures::ground::ID getGround() const;
    Textures::building::ID getBuilding() const;

    float getMoveSpeed() const;
    float getRotation() const;
    bool isCrossable() const;

    void setGround(const Textures::ground::ID &id);
    void setBuilding(const Textures::building::ID &id);

    void setMoveSpeed(const float &speed);
    void setRotation(const float &rotation);
    void setcrossable(const bool &cross);

    float rotate;
    Textures::ground::ID ground;

private:
    sf::Sprite sprite;

    Textures::building::ID building;

    float moveSpeed;
    bool crossable;

};


#endif //LIFPROJET_TILE_HPP

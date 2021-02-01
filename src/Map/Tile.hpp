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

    int getMoveSpeed() const;
    bool isCrossable() const;

    void setGround(const Textures::ground::ID &id);
    void setBuilding(const Textures::building::ID &id);

    void setMoveSpeed(const int &speed);
    void setcrossable(const bool &cross);

private:
    sf::Sprite sprite;

    Textures::ground::ID ground;
    Textures::building::ID building;

    int moveSpeed;
    bool crossable;

};


#endif //LIFPROJET_TILE_HPP

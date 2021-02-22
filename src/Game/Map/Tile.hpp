//
// Created by thibaut on 01/02/2021.
//

#ifndef LIFPROJET_TILE_HPP
#define LIFPROJET_TILE_HPP

#include "../../Game/Resources/ResourceIdentifiers.hpp"
#include "SFML/Graphics/Sprite.hpp"

class Tile {
public:
    Tile();

    Textures::Ground::ID getGround() const;

    float getMoveSpeed() const;
    float getRotation() const;
    bool isCrossable() const;

    void setGround(const Textures::Ground::ID &id);

    void setMoveSpeed(const float &speed);

    void paint(const Textures::Ground::ID &id,const float &rotation );
    void setRotation(const float &rotation);
    void setcrossable(const bool &cross);

private:
    Textures::Ground::ID ground;

    float rotate;
    float moveSpeed;
    bool crossable;

};

#endif //LIFPROJET_TILE_HPP

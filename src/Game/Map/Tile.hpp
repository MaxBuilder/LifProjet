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

    sf::Vector2i getGround() const;
    sf::Vector2i getTop() const;

    bool haveTop() const;

    float getMoveSpeed() const;
    float getRotation() const;
    bool isCrossable() const;

    void paint(const sf::Vector2i &id,const float &rotation);
    void setRotation(const float &rotation);

private:
    sf::Vector2i ground, top;

    float rotate;
    float moveSpeed,topMoveSpeed;
    bool crossable, topCrossable, mHaveTop;

};

#endif //LIFPROJET_TILE_HPP

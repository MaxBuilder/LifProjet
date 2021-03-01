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

    float getMoveSpeed() const;
    float getRotation() const;
    bool isCrossable() const;

    void setGround(const sf::Vector2i &id);

    void setMoveSpeed(const float &speed);

    void paint(const sf::Vector2i &id,const float &rotation);
    void setRotation(const float &rotation);
    void setcrossable(const bool &cross);

private:
    sf::Vector2i ground, top;

    float rotate;
    float moveSpeed,topMoveSpeed;
    bool crossable, topCrossable;

};

#endif //LIFPROJET_TILE_HPP

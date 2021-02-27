//
// Created by 33771 on 23/02/2021.
//

#ifndef LIFPROJET_SOLDIER_HPP
#define LIFPROJET_SOLDIER_HPP

#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/CircleShape.hpp>
#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Graphics/RenderStates.hpp>

#include "Entity.hpp"
#include "../Resources/ResourceIdentifiers.hpp"
#include "../Resources/ResourceHolder.hpp"


class Soldier : public Entity {

public:
    enum Team {
        BlueTeam,
        RedTeam,
        TypeCount
    };

public:
    Soldier(Team team, const TextureHolder& textures, const FontHolder& fonts);

    Team getTeam();
    //virtual sf::FloatRect getBoundingBox();

    void setDirection(sf::Vector2f velocity);
    void setDirection(float vx, float vy);
    sf::Vector2f getDirection() const;

    float getSpeed() const;
    void setSpeed(float speed);

    void remove() override;

private:
    virtual void drawCurrent(sf::RenderTarget& target, sf::RenderStates states) const;
    virtual void updateCurrent(sf::Time dt);

private:
    sf::Sprite mSprite;
    //sf::CircleShape mTemp;
    float mSpeed;
    sf::Vector2f mDirection;
    Team mTeam;

};


#endif //LIFPROJET_SOLDIER_HPP

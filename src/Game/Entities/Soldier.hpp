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
#include "../../Core/Util/Utility.hpp"
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

    void setTarget(Soldier* target);
    void dropTarget();
    Soldier* getTarget();

    void seekTarget();
    void fleeTarget();

private:
    virtual void drawCurrent(sf::RenderTarget& target, sf::RenderStates states) const;
    virtual void updateCurrent(sf::Time dt); // Defines behavior of entity (before specialization)

private:
    sf::Sprite mSprite;

    float mSpeed;
    sf::Vector2f mDirection;
    Team mTeam;

    Soldier * mTargeted;

};


#endif //LIFPROJET_SOLDIER_HPP

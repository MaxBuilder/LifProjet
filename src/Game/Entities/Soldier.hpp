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
#include "../../Core/Util/Rand.hpp"
#include "../Resources/ResourceIdentifiers.hpp"
#include "../Resources/ResourceHolder.hpp"


class Soldier : public Entity {

public:
    enum Team {
        BlueTeam,
        RedTeam,
        TypeCount
    };

    enum Action {
        None,
        Move, // Attackers -> go to objective | Defenders -> roam defended zone
        Seek,
        Flee,
        Attack,
        Override
    };

public:
    Soldier(Team team, const TextureHolder& textures, const FontHolder& fonts);

    Team getTeam();
    Action getAction();
    void setAction(Action act);

    void updateAttack(sf::Time dt);
    void updateDefense(sf::Time dt);

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
    void attackTarget();
    void roam(sf::Time dt);

    void remove() override;
    void init(); // Initiate mOrigin variable (for defense reference)
    void resetTravelledDistance();

    // Testing functions
    void dontMove() { mAction = None; }

private:
    virtual void drawCurrent(sf::RenderTarget& target, sf::RenderStates states) const;
    virtual void updateCurrent(sf::Time dt); // Defines behavior of entity (before specialization)

    sf::Vector2f randomDirection(); // Helper to roam();

private:
    // Needs adding of personal stats (atk, def, per, sp)

    sf::Sprite mSprite;

    float mSpeed;
    sf::Vector2f mDirection;
    sf::Vector2f mOrigin; // Point where entity is instantiated
    float mTravelled;
    int mDistance;
    Team mTeam;
    Action mAction;

    Soldier * mTargeted;

};


#endif //LIFPROJET_SOLDIER_HPP

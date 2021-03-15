//
// Created by 33771 on 23/02/2021.
//

#ifndef LIFPROJET_SOLDIER_HPP
#define LIFPROJET_SOLDIER_HPP

#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/CircleShape.hpp>
#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Graphics/RenderStates.hpp>
#include <SFML/System/Clock.hpp>

#include "Entity.hpp"
#include "../../Core/Util/Utility.hpp"
#include "../../Core/Util/Rand.hpp"
#include "../Resources/ResourceIdentifiers.hpp"
#include "../Resources/ResourceHolder.hpp"


class Soldier : public Entity {

public:

    enum Action {
        None,
        Moving, // Attackers -> go to objective | Defenders -> roam defended zone
        Seeking,
        Fleeing,
        Attacking,
        Calling,
        Helping,
        Override
    };

public:
    Soldier(Team team, const TextureHolder& textures, const FontHolder& fonts, bool big = false);

    void changeBonus(Entity::Bonus bonus);
    Entity::Bonus getBonus();

    Action getAction();
    void setAction(Action act);

    void updateAttack(sf::Time dt);
    void updateDefense(sf::Time dt);

    void setDirection(sf::Vector2f velocity);
    void setDirection(float vx, float vy);
    sf::Vector2f getDirection() const;

    sf::Vector2f getOrigin();

    float getSpeed() const;
    void setSpeed(float speed);

    void setTarget(Soldier* target);
    void dropTarget();
    Soldier* getTarget();

    void seekTarget();
    void fleeTarget();
    void attackTarget();
    void roam(sf::Time dt);
    void helpAlly(Soldier * ally);
    void helpRequested(Soldier * ally);

    void remove() override;
    void init(); // Initiate mOrigin variable (for defense reference)
    void resetTravelledDistance();

    // Testing functions
    void dontMove() { mAction = None; }

private:
    void drawCurrent(sf::RenderTarget& target, sf::RenderStates states) const override;
    void updateCurrent(sf::Time dt) override; // Defines behavior of entity (before specialization)

    sf::Vector2f randomDirection(); // Helper to roam();

public:
    bool isAvailable;

private:
    // Needs adding of personal stats (atk, def, per, sp)
    bool isBigBitch; // Temporary
    Entity::Bonus mBonus;
    sf::Sprite mSprite;
    sf::Sprite mGlow;
    sf::Text mLife;
    sf::IntRect mSpriteRect;
    sf::Time mSpriteTime;

    float mSpeedBonus, mSpeedBase;
    float mDamages;
    sf::Vector2f mDirection;
    sf::Vector2f mOrigin; // Point where entity is instantiated
    float mTravelled; // Distance travelled
    int mDistance; // Distance to travel
    sf::Clock mEntityClock;
    Action mAction;

    Soldier * mTargeted;

};


#endif //LIFPROJET_SOLDIER_HPP

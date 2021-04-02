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
#include "../Pathfinding//Pathfinding.hpp"
#include "../../Core/Util/Utility.hpp"
#include "../../Core/Util/Rand.hpp"
#include "../Resources/ResourceIdentifiers.hpp"
#include "../Resources/ResourceHolder.hpp"
#include "../Map/TilesMap.hpp"


class Soldier : public Entity {

public:

    enum Action {
        None,
        Moving, // Attackers -> go to objective | Defenders -> roam defended mZone
        Seeking,
        Fleeing,
        Attacking,
        Calling,
        Leading,
        WithSquad,
        Assaulting,
        Override
    };

public:
    Soldier(int id, Team team, const TextureHolder& textures, const FontHolder& fonts, Pathfinding& Astar, CommandQueue& commandQueue);

    int getId() const;

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

    sf::Vector2f getVelocity();
    void setVelocity(sf::Vector2f dpl);

    void setTarget(Entity* target);
    void dropTarget();
    void setLeader(Soldier* leader);
    void dropLeader();
    Entity* getTarget();

    void travel();
    void seekTarget();
    void seekTarget(sf::Vector2f pos);
    void fleeTarget();
    void attackTarget();
    void roam(sf::Time dt);
    void createTeam(int senderId);

    void remove() override;
    void init(); // Initiate mOrigin variable (for defense reference)
    void resetTravelledDistance();

private:
    void drawCurrent(sf::RenderTarget& target, sf::RenderStates states) const override;
    void updateCurrent(sf::Time dt) override; // Defines behavior of entity (before specialization)

    sf::Vector2f randomDirection(); // Helper to roam();

private:
    std::shared_ptr<Pathfinding> mAstar;

    // Needs adding of personal stats (atk, def, per, sp)

    Entity::Bonus mBonus;
    sf::Sprite mSprite;
    sf::Sprite mGlow;
    sf::Text mLife;
    sf::IntRect mSpriteRect;
    sf::Time mSpriteTime;

    std::vector<sf::Vector2f> mPath;
    float mSpeedBonus, mSpeedBase;
    float mDamages;
    sf::Vector2f mDirection;
    sf::Vector2f mVelocity;
    sf::Vector2f mOrigin; // Point where entity is instantiated
    float mTravelled; // Distance travelled
    int mDistance; // Distance to travel
    sf::Clock mEntityClock, mAstarDuration;
    Action mAction;
    bool prev;

    Entity * mTargeted;
    Soldier * mLeader;
    int mId;

public:
    int mTargetInSight;
    int mAllyInSight;

    int nbRequested;
    int nbResponse;
    int mSquadSize;
    int nbInPlace;
    std::vector<int> mSquadIds;
};


#endif //LIFPROJET_SOLDIER_HPP

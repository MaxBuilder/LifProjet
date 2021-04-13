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
#include "../../Core/Util/Debug.hpp"
#include "../Resources/ResourceHolder.hpp"
#include "../Map/TilesMap.hpp"
#include "Projectile.hpp"


class Soldier : public Entity {

public:

    enum Action {
        None,
        Moving,
        Seeking,
        Attacking,
        Calling,
        Leading,
        WithSquad,
        Assaulting,
        DefendingCastle
    };

    enum Debug {
        Id, cAction, Life
    };

public:
    Soldier(int id, EntityInfo::ID soldierType, EntityInfo::Team team, sf::Vector2i objectif, const TextureHolder& textures, const FontHolder& fonts, Pathfinding& Astar, CommandQueue& commandQueue);

    int getId() const;

    void switchDebugDisplay();

    void changeBonus(EntityInfo::ID bonus);

    Action getAction();
    void setAction(Action act);

    void updateSprite(sf::Time dt);
    void updateAttack(sf::Time dt);
    void updateDefense(sf::Time dt);

    void setDirection(sf::Vector2f velocity);
    void setDirection(float vx, float vy);
    sf::Vector2f getDirection();

    sf::Vector2f getVelocity();
    void setVelocity(sf::Vector2f dpl);

    void setTarget(Entity* target);
    void setLeader(Soldier* leader);

    void travel();
    void seekTarget(sf::Vector2f pos = sf::Vector2f(-1,-1));
    void attackTarget();
    void roam(sf::Time dt);
    void createTeam(int senderId);

    void remove() override;
    void init(); // Initiate mOrigin variable (for defense reference)

    Entity* getTarget();
    int getDamage();

private:
    void drawCurrent(sf::RenderTarget& target, sf::RenderStates states) const override;
    void updateCurrent(sf::Time dt) override; // Defines behavior of entity (before specialization)

    sf::Vector2f randomDirection(); // Helper to roam();

private:
    Pathfinding& mPathfinding;

    int mId;
    EntityInfo::ID mSoldierType;
    EntityInfo::ID mBonus;
    sf::Time mEntityTime;
    Action mAction;
    int mRange;

    Entity * mTargeted;
    Soldier * mLeader;

    sf::Sprite mSprite;
    sf::Sprite mGlow;
    sf::Text mDisplayID,mDisplayAction;
    sf::RectangleShape backLife,frontLife;
    sf::IntRect mSpriteRect;
    sf::Time mSpriteTime;

    Debug mDisplayType;

    std::vector<sf::Vector2f> mPath;
    float mSpeedBonus, mSpeedBase;
    int mDamages;
    sf::Vector2f mObjectif;
    sf::Vector2f mDirection;
    sf::Vector2f mVelocity;
    sf::Vector2f mOrigin; // Point where entity is instantiated
    float mTravelled; // Distance travelled
    int mDistance; // Distance to travel
    sf::Clock mAstarDuration;

    bool prev;
    bool sendAck;

    std::string name[10] = {"None", "Moving", "Seeking", "Attacking", "Calling", "Leading", "WithSquad", "Assaulting", "DefendingCastle"};

public:
    // Special behavior variables :
    bool usePathFinding;
    int mTargetInSight;
    int mAllyInSight;
    sf::Vector2f closetInSightDirection; // Keep last direction to launch assault

    int nbRequested;
    int nbResponse;
    int mSquadSize;
    int nbInPlace;
    std::vector<int> mSquadIds;
};


#endif //LIFPROJET_SOLDIER_HPP

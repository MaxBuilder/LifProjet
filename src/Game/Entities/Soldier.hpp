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

#include "../../Core/Util/Utility.hpp"
#include "../../Core/Util/Debug.hpp"
#include "../../Core/Util/Rand.hpp"
#include "../Pathfinding//Pathfinding.hpp"
#include "../Resources/ResourceIdentifiers.hpp"
#include "../Resources/ResourceHolder.hpp"
#include "../Map/TilesMap.hpp"
#include "Projectile.hpp"
#include "Entity.hpp"

/**
 * @class Soldier
 * @brief Stores all the information of the soldier entities
 */
class Soldier : public Entity {

public:
    /**
     * @enum Action
     * @brief Defines the behavior of the entity
     */
    enum Action {
        Standby,
        Moving,
        Seeking,
        Attacking,
        Calling,
        Leading,
        WithSquad,
        Assaulting,
        DefendingCastle
    };

    /**
     * @enum Debug
     * @brief Defines what is displayed above the soldier (id, action, life)
     */
    enum Debug {
        Id, cAction, Life
    };

public:
    /**
     * @brief Parameter constructor
     * @param id Id
     * @param soldierType Type of soldier
     * @param team Team
     * @param objectif Objective (for attackers)
     * @param textures Texture of the soldier
     * @param fonts Font
     * @param Astar Pathfinding
     * @param commandQueue Reference to the command queue
     */
    Soldier(int id, EntityInfo::ID soldierType, EntityInfo::Team team, sf::Vector2i objectif, const TextureHolder& textures, const FontHolder& fonts, Pathfinding& Astar, CommandQueue& commandQueue);

    /**
     * Returns the id of the entity
     * @return Id
     */
    int getId() const;
    EntityInfo::ID getSoldierType() const;
    /**
     * @brief Changes the actual display
     */
    void switchDebugDisplay();

    /**
     * Changes the bonus of the entity
     * @param bonus Bonus to give
     */
    void changeBonus(EntityInfo::ID bonus);

    /**
     * @brief Returns current action of the entity
     * @return Action
     */
    Action getAction();
    /**
     * @brief Sets the action of the entity
     * @param act Action to set
     */
    void setAction(Action act);

    /**
     * @brief Updates the sprite of the entity
     * @param dt Time interval bewteen the last update
     */
    void updateSprite(sf::Time dt);
    /**
     * @brief Updates the behavior of the attackers (red team)
     * @param dt
     */
    void updateAttack(sf::Time dt);
    /**
     * @brief Updates the behavior of the defenders (blue team)
     * @param dt
     */
    void updateDefense(sf::Time dt);

    /**
     * @brief Sets the direction of the entity
     * @param velocity Direction to set
     */
    void setDirection(sf::Vector2f velocity);
    //sf::Vector2f getDirection();

    /**
     * @brief Returns the current velocity of the entity
     * @return Current velocity
     */
    sf::Vector2f getVelocity();
    /**
     * @brief Sets the velocity of the entity
     * @param dpl New velocity to set
     */
    void setVelocity(sf::Vector2f dpl);

    /**
     * @brief Sets the target of the entity
     * @param target Target to set
     */
    void setTarget(Entity* target);
    /**
     * @brief Sets the leader of the entity
     * @param leader Leader to sert
     */
    void setLeader(Soldier* leader);

    /**
     * @brief Moves the entity relative to it's velocity
     */
    void travel();
    /**
     * @brief Uses pathfinding to lead the unit to it's target
     * @param pos Position where the soldier is headed
     */
    void seekTarget(sf::Vector2f pos = sf::Vector2f(-1,-1));
    /**
     * @brief Checks the soldier's cooldown and damages it's target
     */
    void attackTarget();
    /**
     * @brief (Defenders only) roams around origin point
     * @param dt Time interval since last update
     */
    void roam(sf::Time dt);
    /**
     * @brief Upong receiving a MakeTeam commands, checks soldier current action and sends Accept / Deny
     * @param senderId Id of the Sender
     */
    void createTeam(int senderId);

    /**
     * @brief Remove the entity if dead
     */
    void remove() override;
    /**
     * @brief Initializes origin position for defenders (needed to roam)
     */
    void init(); // Initiate mOrigin variable (for defense reference)

    /**
     * @brief Returns the actual target of the soldier
     * @return Current target
     */
    Entity* getTarget();
    /**
     * @brief Returns the damages of the entity
     * @return Damages
     */
    int getDamage();

private:
    /**
         * @brief Draws the object to the target
         * @param target Target to render the object to
         * @param states Informations on how to render the object
         */
    void drawCurrent(sf::RenderTarget& target, sf::RenderStates states) const override;
    /**
     * @brief Updates behavior, sprite and internal data of soldier according to team (atk / def)
     * @param dt Time interval since last update
     */
    void updateCurrent(sf::Time dt) override; // Defines behavior of entity (before specialization)

    /**
     * @brief Picks a random direction
     * @return Vector of a random direction
     */
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

    // sprites
    sf::Sprite mSprite;
    sf::Sprite mGlow;
    Action mSpriteAction;
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

    std::string name[10] = {"Standby", "Moving", "Seeking", "Attacking", "Calling", "Leading", "WithSquad", "Assaulting", "DefendingCastle"};

public:
    // Special behavior variables :
    bool usePathFinding;
    int mTargetInSight;
    int mAllyInSight;
    sf::Vector2f closetInSightDirection;

    int nbRequested;
    int nbResponse;
    int mSquadSize;
    int nbInPlace;
    std::vector<int> mSquadIds;
};


#endif //LIFPROJET_SOLDIER_HPP

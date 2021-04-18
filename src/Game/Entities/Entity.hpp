//
// Created by 33771 on 23/02/2021.
//

#ifndef LIFPROJET_ENTITY_HPP
#define LIFPROJET_ENTITY_HPP

#include "../Map/EntityInfo.hpp"
#include "../../Core/Scene/SceneNode.hpp"
#include "../../Core/Commands/CommandQueue.hpp"
#include "../../Game/Resources/ResourceIdentifiers.hpp"

/**
 * @class Entity
 * @brief Store all the common information between the entities
 */
class Entity : public SceneNode {

public:
    /**
     * @brief Parameter constructor
     * @param hitPoints Life of the entity
     * @param team Team
     * @param commandQueue Reference to the command queue
     */
    explicit Entity(EntityInfo::ID ID, EntityInfo::Team team, int hitPoints, CommandQueue &commandQueue);

    /**
     * @brief Returns the team of the entity
     * @return Team
     */
    EntityInfo::Team getTeam();
    /**
     * @brief Returns the type of bonus
     * @return Type of the bonus
     */
    EntityInfo::ID getType() const;
    /**
     * @brief Sets the health and the max health
     * @param health New health
     */
    void setHealth(int health);
    /**
     * @brief Returns the border of the entity
     * @return Border
     */
    float getBorder() const;

    /**
     * @brief Heals the entity by a certain amount
     * @param points Life to give to the entity
     */
    void heal(int points);
    /**
     * @brief Damages the entity by a certain amount
     * @param points Life to take from the entity
     */
    void damage(int points);
    /**
     * @brief Destroys the entity (life to 0)
     */
    void destroy();
    /**
     * @brief Virtual version of destroy
     */
    virtual void remove();
    /**
     * Returns if the entity is destroyed
     * @return true is entity deosn't have life points left
     */
    virtual bool isDestroyed() const;

    bool down;

protected:
    /**
     * @brief Updates the entity
     * @param dt Time interval since last update
     */
    void updateCurrent(sf::Time dt) override;

    float mBorder;
    int mHitPoints;
    int mMaxHintPoints;
    EntityInfo::Team mTeam;
    EntityInfo::ID mType;
    CommandQueue& mCommandQueue;

};


#endif //LIFPROJET_ENTITY_HPP

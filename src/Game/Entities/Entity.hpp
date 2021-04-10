//
// Created by 33771 on 23/02/2021.
//

#ifndef LIFPROJET_ENTITY_HPP
#define LIFPROJET_ENTITY_HPP

#include "../Map/EntityInfo.hpp"
#include "../../Core/Scene/SceneNode.hpp"
#include "../../Core/Commands/CommandQueue.hpp"
#include "../../Game/Resources/ResourceIdentifiers.hpp"

class Entity : public SceneNode {

public:
    explicit Entity(int hitPoints, EntityInfo::Team team, CommandQueue &commandQueue);

    EntityInfo::Team getTeam();
    int getHitPoints() const;
    float getBorder() const;
    void heal(int points);
    void damage(int points);
    void destroy();
    virtual void remove();
    virtual bool isDestroyed() const;

    bool down;

protected:
    void updateCurrent(sf::Time dt) override;

    float mBorder;
    int mHitPoints;
    int mMaxHintPoints;
    EntityInfo::Team mTeam;
    CommandQueue& mCommandQueue;

};


#endif //LIFPROJET_ENTITY_HPP

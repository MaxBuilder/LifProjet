//
// Created by 33771 on 23/02/2021.
//

#ifndef LIFPROJET_ENTITY_HPP
#define LIFPROJET_ENTITY_HPP

#include "../../Core/Scene/SceneNode.hpp"
#include "../../Core/Commands/CommandQueue.hpp"


class Entity : public SceneNode {

public:

    enum Team {
        BlueTeam,
        RedTeam,
        TypeCount
    };

    enum Bonus{
        None,
        Castle,
        Village
    };

    bool down;

    explicit Entity(int hitPoints, Team team, CommandQueue &commandQueue);

    Team getTeam();
    int getHitPoints() const;
    float getBorder() const;
    void heal(int points);
    void damage(int points);
    void destroy();
    virtual void remove();
    virtual bool isDestroyed() const;

protected:
    virtual void updateCurrent(sf::Time dt);

protected:
    float mBorder;
    int mHitPoints;
    Team mTeam;
    CommandQueue& mCommandQueue;

};


#endif //LIFPROJET_ENTITY_HPP

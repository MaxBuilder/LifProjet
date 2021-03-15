//
// Created by 33771 on 23/02/2021.
//

#ifndef LIFPROJET_ENTITY_HPP
#define LIFPROJET_ENTITY_HPP

#include "../../Core/Scene/SceneNode.hpp"


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

    explicit Entity(int hitPoints, Team team);

    Team getTeam();
    int getHitPoints() const;
    void heal(int points);
    void damage(int points);
    void destroy();
    virtual void remove();
    virtual bool isDestroyed() const;

protected:
    virtual void updateCurrent(sf::Time dt);

protected:
    int mHitPoints;
    Team mTeam;

};


#endif //LIFPROJET_ENTITY_HPP

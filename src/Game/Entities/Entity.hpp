//
// Created by 33771 on 23/02/2021.
//

#ifndef LIFPROJET_ENTITY_HPP
#define LIFPROJET_ENTITY_HPP

#include "../../Core/Scene/SceneNode.hpp"


class Entity : public SceneNode {

public:
    explicit Entity(int hitPoints);

    int getHitPoints() const;
    void heal(int points);
    void damage(int points);
    void destroy();
    virtual void remove();
    virtual bool isDestroyed() const;

protected:
    virtual void updateCurrent(sf::Time dt);

private:
    int mHitPoints;

};


#endif //LIFPROJET_ENTITY_HPP

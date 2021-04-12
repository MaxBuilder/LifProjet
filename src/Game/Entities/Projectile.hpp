//
// Created by thibaut on 11/04/2021.
//

#ifndef LIFPROJET_PROJECTILE_HPP
#define LIFPROJET_PROJECTILE_HPP

#include "Entity.hpp"
#include "../../Core/Util/Utility.hpp"
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Graphics/RenderStates.hpp>

class Projectile : public SceneNode {

public :
    Projectile(sf::Vector2f position, Entity *target,const sf::Texture& texture, int degats);

    bool onTarget();
    bool detachRequiere();

private :
    void updateCurrent(sf::Time dt) override;
    void drawCurrent(sf::RenderTarget& target, sf::RenderStates states) const override;

    void updateMovement(sf::Time dt);

    int mDegats;
    float mRotation;
    float mVelocity;
    bool onGround;

    Entity *mTarget;
    sf::Vector2f mTargetPosition;
    sf::Sprite mSprite;
    sf::Time timeOnGround;

};



#endif //LIFPROJET_PROJECTILE_HPP

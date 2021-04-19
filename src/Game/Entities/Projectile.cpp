//
// Created by thibaut on 11/04/2021.
//

#include "Projectile.hpp"
#include <iostream>

Projectile::Projectile(sf::Vector2f position, Entity *target, const sf::Texture& texture, int damages){
    mDamages = damages;
    mRotation = 0;
    mVelocity = 80.f;
    onGround = false;

    mTarget = target;
    setPosition(position);
    mTargetPosition = mTarget->getPosition();
    mSprite.setTexture(texture);
    setOrigin(mSprite.getLocalBounds().left/2.f,mSprite.getLocalBounds().top/2.f);
    mSprite.scale(0.5f,0.5f);
    mRotation = angle(mTargetPosition,getPosition());
    mSprite.setRotation(mRotation);
    timeOnGround = sf::milliseconds(3000);
}

bool Projectile::onTarget(){
    return (distance(getPosition(),mTargetPosition) < 4) or (distance(getPosition(),mTarget->getPosition()) < 15);
}

void Projectile::updateCurrent(sf::Time dt){
    if (onTarget() and not onGround ){

        if (distance(getPosition(),mTarget->getPosition()) < 15)
            mTarget->damage(mDamages);

        onGround = true;
    }
    else if (onGround)
        timeOnGround = sf::milliseconds(timeOnGround.asMilliseconds() - dt.asMilliseconds());

    else{
        updateMovement(dt);
    }
}

bool Projectile::detachRequiere() {
    return timeOnGround.asMilliseconds() < 0;
}

void Projectile::updateMovement(sf::Time dt){
    sf::Vector2f dpl = mTargetPosition - getPosition() ;
    dpl = dpl / norm(dpl);
    move(dpl*mVelocity*dt.asSeconds());

}

void Projectile::drawCurrent(sf::RenderTarget& target, sf::RenderStates states)const{
    target.draw(mSprite, states);
}


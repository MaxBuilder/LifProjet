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

/**
 * @class Projectile
 * @brief Stores all the information about projectiles sent by entities
 */
class Projectile : public SceneNode {

public :
    /**
     * @brief Parameter constructor
     * @param position Starting position
     * @param target Target of the projectile
     * @param texture Texture to give
     * @param damages Damages when hitting targets
     */
    Projectile(sf::Vector2f position, Entity *target,const sf::Texture& texture, int damages);

    /**
     * @brief Returns if the projectile is on the target
     * @return If projectile hit his target
     */
    bool onTarget();
    /**
     * @brief Returns if the projectile needs to be removed
     * @return true if needs removal
     */
    bool detachRequiere();

private :
    /**
     * @brief Updates the projectile
     * @param dt Time interval since last update
     */
    void updateCurrent(sf::Time dt) override;
    /**
     * @brief Draws the projectile to the target
     * @param target Target to render the object to
     * @param states Informations on how to render the object
     */
    void drawCurrent(sf::RenderTarget& target, sf::RenderStates states) const override;
    /**
     * @brief Updates the orientation of the projectile
     * @param dt Time interval since last update
     */
    void updateMovement(sf::Time dt);

private:
    int mDamages;
    float mRotation;
    float mVelocity;
    bool onGround;

    Entity *mTarget;
    sf::Vector2f mTargetPosition;
    sf::Sprite mSprite;
    sf::Time timeOnGround;

};



#endif //LIFPROJET_PROJECTILE_HPP

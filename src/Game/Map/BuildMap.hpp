//
// Created by 33771 on 18/02/2021.
//

#ifndef LIFPROJET_BUILDMAP_HPP
#define LIFPROJET_BUILDMAP_HPP

#include "../../Game/Resources/ResourceIdentifiers.hpp"
#include "SFML/Graphics/Sprite.hpp"

class BuildMap {
public:
    BuildMap(Textures::building::ID ID,sf::IntRect position, float rotation );

    Textures::building::ID getID() const;
    sf::IntRect getPosition() const;
    void setPosition(sf::IntRect rect);

    sf::Sprite& getSprite();
    sf::Sprite getConstSprite() const;

    float mRotation;

private:
    Textures::building::ID mId;

    sf::IntRect mPosition;
    sf::Sprite mSprite;

};


#endif //LIFPROJET_BUILDMAP_HPP

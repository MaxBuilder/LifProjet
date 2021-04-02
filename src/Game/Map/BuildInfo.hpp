//
// Created by 33771 on 18/02/2021.
//

#ifndef LIFPROJET_BUILDINFO_HPP
#define LIFPROJET_BUILDINFO_HPP

#include "../../Game/Resources/ResourceIdentifiers.hpp"
#include "SFML/Graphics/Rect.hpp"

class BuildInfo {
public:
    BuildInfo(Buildings::ID ID, sf::IntRect position);

    Buildings::ID getID() const;
    sf::IntRect getPosition() const;
    void setPosition(sf::IntRect rect);

private:
    Buildings::ID mId;
    sf::IntRect mPosition;
};


#endif //LIFPROJET_BUILDINFO_HPP

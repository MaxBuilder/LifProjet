//
// Created by 33771 on 18/02/2021.
//

#ifndef LIFPROJET_BUILDINFO_HPP
#define LIFPROJET_BUILDINFO_HPP

#include "../../Game/Resources/ResourceIdentifiers.hpp"
#include "SFML/Graphics/Rect.hpp"

class BuildInfo {
public:
    BuildInfo(EntityInfo::ID ID,EntityInfo::Team team, sf::IntRect position);

    EntityInfo::ID getID() const;
    EntityInfo::Team getTeam() const;
    sf::IntRect getPosition() const;
    void setPosition(sf::IntRect rect);

private:
    EntityInfo::ID mId;
    EntityInfo::Team mTeam;
    sf::IntRect mPosition;
};


#endif //LIFPROJET_BUILDINFO_HPP

//
// Created by thibaut on 28/03/2021.
//

#ifndef LIFPROJET_ENTITYINFO_HPP
#define LIFPROJET_ENTITYINFO_HPP

#include "../../Game/Resources/ResourceIdentifiers.hpp"
#include "SFML/Graphics/Rect.hpp"

class EntityInfo {

public:

    enum Team {
        Blue,
        Red
    };

    enum Type {
        Building,
        Soldier
    };

    enum ID {
        None,
        Knight,
        Archer,
        Tank,
        Castle,
        Village,
        Barrier
    };

    EntityInfo(sf::Vector2f position, EntityInfo::ID id, EntityInfo::Team team, EntityInfo::Type type);

    sf::Vector2f getPosition() const;
    EntityInfo::ID getID() const;
    EntityInfo::Type getType() const;
    EntityInfo::Team getTeam() const;

private:
    sf::Vector2f mPosition;
    EntityInfo::ID mId;
    EntityInfo::Type mType;
    EntityInfo::Team mTeam;


};


#endif //LIFPROJET_ENTITYINFO_HPP

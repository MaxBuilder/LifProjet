//
// Created by thibaut on 28/03/2021.
//

#ifndef LIFPROJET_ENTITYINFO_HPP
#define LIFPROJET_ENTITYINFO_HPP

#include "../../Game/Resources/ResourceIdentifiers.hpp"
#include "SFML/Graphics/Rect.hpp"

/**
 * @class EntityInfo
 * @brief Stores all the information about an entity
 */
class EntityInfo {

public:

    /**
     * @enum Team
     * @brief An entity can be in Blue or Red team
     */
    enum Team {
        Blue,
        Red
    };

    /**
     * @enum Type
     * @brief An entity can be a Soldier or a Building
     */
    enum Type {
        Building,
        Soldier
    };

    /**
     * @enum ID
     * @brief All the different kind of building and soldier
     */
    enum ID {
        None,
        Knight,
        Archer,
        Tank,
        Castle,
        Village,
        Barrier
    };

    /**
    * @brief Contructor of EntityInfo
    * @param Position of the entity in the map
    * @param Id of the entity ( enum )
    * @param Team of the entity( enum )
    * @param Type of the entity ( enum )
    */
    EntityInfo(sf::Vector2f position, EntityInfo::ID id, EntityInfo::Team team, EntityInfo::Type type);

    /**
     * @brief Get the position of the entity
     * @return Coordinate of the position
     */
    sf::Vector2f getPosition() const;
    /**
     * @brief Get the Id of the entity
     * @return Id of entity ( enum )
     */
    EntityInfo::ID getID() const;
    /**
     * @brief Get the type of the entity
     * @return Type of entity ( enum )
     */
    EntityInfo::Type getType() const;
    /**
     * @brief Get the team of the entity
     * @return Team of entity ( enum )
     */
    EntityInfo::Team getTeam() const;

private:
    sf::Vector2f mPosition;
    EntityInfo::ID mId;
    EntityInfo::Type mType;
    EntityInfo::Team mTeam;


};


#endif //LIFPROJET_ENTITYINFO_HPP

//
// Created by thibaut on 28/03/2021.
//

#ifndef LIFPROJET_ENTITYINFO_HPP
#define LIFPROJET_ENTITYINFO_HPP

#include "../../Game/Resources/ResourceIdentifiers.hpp"
#include "SFML/Graphics/Rect.hpp"

class EntityInfo {
public:
    EntityInfo(sf::Vector2f position, Editor::Entity type, Editor::Tool team);

    [[nodiscard]] sf::Vector2f getPosition() const;
    [[nodiscard]] Editor::Entity getType() const;
    [[nodiscard]] Editor::Tool getTeam() const;

private:
    sf::Vector2f mPosition;
    Editor::Entity mType;
    Editor::Tool mTeam;


};


#endif //LIFPROJET_ENTITYINFO_HPP

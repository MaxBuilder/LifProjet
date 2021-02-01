//
// Created by thibaut on 01/02/2021.
//

#ifndef LIFPROJET_TILESMAP_HPP
#define LIFPROJET_TILESMAP_HPP

#include "Tile.hpp"
#include "SFML/Graphics/Drawable.hpp"
#include "SFML/Graphics/Transformable.hpp"
#include "SFML/Graphics/Sprite.hpp"
#include "../Resources/ResourceIdentifiers.hpp"
#include "SFML/Graphics/Texture.hpp"
#include "SFML/Window/Event.hpp"
#include "SFML/Graphics/RenderWindow.hpp"

class TilesMap : public sf::Drawable, public sf::Transformable{
public:
    TilesMap(mapMode::mode type,const sf::Texture &text);

    Tile& getTile(int x, int y);
    virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;
    void handleEvent(const sf::Event& event,const sf::RenderWindow& window);

    void setGroundSelection(Textures::ground::ID id);

private:
    void setSpriteRect(Textures::ground::ID id, Tile& tile);

    Tile grid_id[64][36];
    sf::Vector2i origin;
    sf::Texture texture;
    int size;

    Textures::ground::ID ground_selection;
};


#endif //LIFPROJET_TILESMAP_HPP

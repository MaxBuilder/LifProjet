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
#include <iostream>
#include <fstream>

class TilesMap : public sf::Drawable, public sf::Transformable{
public:
    TilesMap(mapMode::mode type,const sf::Texture &text);

    void save(const std::string &file) const;
    void load(const std::string &file);

    Tile& getTile(int x, int y);
    virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;
    void handleEvent(const sf::Event& event,const sf::RenderWindow& window);

    void setGroundSelection(Textures::ground::ID id);
    void setRotation(float rotation);

private:
    static void setSpriteRect(float rotation, Textures::ground::ID id, Tile& tile);

    Tile grid_id[64][36];
    sf::Vector2i origin;
    sf::Texture texture;
    float size;

    Textures::ground::ID ground_selection;
    float rotate;
};


#endif //LIFPROJET_TILESMAP_HPP

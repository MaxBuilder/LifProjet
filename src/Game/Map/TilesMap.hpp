//
// Created by thibaut on 01/02/2021.
//

#ifndef LIFPROJET_TILESMAP_HPP
#define LIFPROJET_TILESMAP_HPP

#include "Tile.hpp"
#include "Paint.hpp"
#include "SFML/Graphics/Drawable.hpp"
#include "SFML/Graphics/Transformable.hpp"
#include "SFML/Graphics/Sprite.hpp"
#include "../../Game/Resources/ResourceIdentifiers.hpp"
#include "SFML/Graphics/Texture.hpp"
#include "SFML/Window/Event.hpp"
#include "SFML/Graphics/RenderWindow.hpp"
#include <iostream>
#include <fstream>
#include <list>

struct action {

    Textures::ground::ID id;
    map::tool tool;
    float rotation;
    sf::Vector2i coordinate;

};

class TilesMap : public sf::Drawable, public sf::Transformable{

public:
    TilesMap(map::mode type,const sf::Texture &text, sf::Vector2u Wsize);

    void save(const std::string &file) const;
    void load(const std::string &file);

    Tile& getTile(int x, int y);
    virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;
    void handleEvent(const sf::Event& event,const sf::RenderWindow& window);

    void setGroundSelection(Textures::ground::ID id);
    void setRotation(float rotation);
    void setTool(map::tool tmpTool);

    void Undo();

private:

    void paintSquare3(float rotation, Textures::ground::ID id, sf::Vector2i coordinate);
    void paintCircle5(float rotation, Textures::ground::ID id, sf::Vector2i coordinate);
    void paintFill(float rotation, Textures::ground::ID id, sf::Vector2i coordinate);

    void recPaintFill(float rotation, Textures::ground::ID id, sf::Vector2i coordinate, bool* isPaint);

    Tile grid_id[64][36];

    sf::Vector2i origin;
    sf::Vector2u windowSize;
    sf::Vector2i lastTileUpdate;
    sf::Texture texture;

    float size;
    float rotate;

    Textures::ground::ID ground_selection;
    Textures::ground::ID lastGround;
    map::tool tool;

    std::list<action> undo;
};


#endif //LIFPROJET_TILESMAP_HPP

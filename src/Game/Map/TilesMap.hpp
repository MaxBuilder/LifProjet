//
// Created by thibaut on 01/02/2021.
//

#ifndef LIFPROJET_TILESMAP_HPP
#define LIFPROJET_TILESMAP_HPP

#include "Tile.hpp"
#include "Building.hpp"
#include "../../Game/Resources/ResourceIdentifiers.hpp"

#include "SFML/Graphics/Drawable.hpp"
#include "SFML/Graphics/Transformable.hpp"
#include "SFML/Graphics/Sprite.hpp"
#include "SFML/Graphics/Texture.hpp"
#include "SFML/Window/Event.hpp"
#include "SFML/Graphics/RenderWindow.hpp"

#include <iostream>
#include <fstream>
#include <list>

class TilesMap : public sf::Drawable, public sf::Transformable{

public:
    TilesMap(const sf::Texture &texture, float blocSize);

    void clear();
    void save(const std::string &file) const;
    void load(const std::string &file);
    void setDrawBuildings(bool draw);

    Tile& getTile(int x, int y);
    std::pair<std::vector<Building>::iterator,
            std::vector<Building>::iterator> getBuildingsIt();

    void addBuildings(Building build);
    void supBuildings(std::vector<Building>::iterator it);

    float getBlockSize() const;

    void draw(sf::RenderTarget& target, sf::RenderStates states)const override;

private:

    Tile grid_id[64][36];
    std::vector<Building> mBuildings;
    sf::Texture texture;
    sf::Sprite mSprite;

    float mBlockSize;

    bool mDrawBuildings;

};


#endif //LIFPROJET_TILESMAP_HPP

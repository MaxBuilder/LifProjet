//
// Created by thibaut on 01/02/2021.
//

#ifndef LIFPROJET_TILESMAP_HPP
#define LIFPROJET_TILESMAP_HPP

#include "Tile.hpp"
#include "BuildInfo.hpp"
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
    Tile& getTile(sf::Vector2f position);
    std::pair<std::vector<BuildInfo>::iterator,
            std::vector<BuildInfo>::iterator> getBuildingsIt();

    void addBuildings(BuildInfo build);
    void supBuildings(std::vector<BuildInfo>::iterator it);

    float getBlockSize() const;

    void draw(sf::RenderTarget& target, sf::RenderStates states)const override;

private:

    Tile grid_id[64][36];
    std::vector<BuildInfo> mBuildings;
    sf::Texture texture;
    sf::Sprite mSprite;

    float mBlockSize;
    float mBounds;

    bool mDrawBuildings;

};


#endif //LIFPROJET_TILESMAP_HPP

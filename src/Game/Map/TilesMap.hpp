//
// Created by thibaut on 01/02/2021.
//

#ifndef LIFPROJET_TILESMAP_HPP
#define LIFPROJET_TILESMAP_HPP

#include "Tile.hpp"
#include "EntityInfo.hpp"
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
    TilesMap(const sf::Texture &mapTexture, const sf::Texture &entitiesTexture, float blocSize);

    void clear();
    void save(const std::string &file) const;
    void load(const std::string &file);

    Tile& getTile(int x, int y);
    Tile& getTile(sf::Vector2f position);
    Tile& getTile(sf::Vector2i position);

    std::pair<std::vector<EntityInfo>::iterator,
            std::vector<EntityInfo>::iterator> getBuildingsIt();
    std::pair<std::vector<EntityInfo>::iterator,
            std::vector<EntityInfo>::iterator> getEntitiesIt();

    void addBuildings(EntityInfo build);
    void supBuildings(std::vector<EntityInfo>::iterator it);

    void addEntity(EntityInfo entity);
    void supEntity(std::vector<EntityInfo>::iterator it);

    float getBlockSize() const;

    void draw(sf::RenderTarget& target, sf::RenderStates states)const override;

private:

    Tile grid_id[64][36];
    sf::Texture texture;
    sf::Sprite mSprite;
    sf::Sprite mEntitySprite;

    std::vector<EntityInfo> mBuildings;
    std::vector<EntityInfo> mEntities;

    float mBlockSize;
    float mBounds;

    bool mDrawEntity;

};


#endif //LIFPROJET_TILESMAP_HPP

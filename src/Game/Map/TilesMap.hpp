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

/**
 * @class TilesMap
 * @brief Stores all the information about the map
 */
class TilesMap : public sf::Drawable, public sf::Transformable{

public:
    /**
    * @brief Constructor use by GameState
    * @param Texture use by the tiles
    * @param Size of each Tile
    */
    TilesMap(const sf::Texture &texture, float blocSize);
    /**
    * @brief Constructor use by mapEditor State (display vector of entities)
    * @param Texture use by the tiles
    * @param Texture use by the entities
    * @param Size of each Tile
    */
    TilesMap(const sf::Texture &mapTexture, const sf::Texture &entitiesTexture, float blocSize);

    /**
    * @brief Suppres all the information of the map
    */
    void clear();
    /**
    * @brief Save all the information of the map
    * @param Path of the save file
    */
    void save(const std::string &file) const;
    /**
    * @brief Load information from the save file
    * @param Path of the save file
    */
    void load(const std::string &file);

    /**
    * @brief Get a Tile
    * @param X coordinate
    * @param Y coordinate
    * @return Tile of the given coordinate
    */
    Tile& getTile(int x, int y);
    /**
    * @brief Get a Tile
    * @param Float vector2 of position
    * @return Tile of the given coordinate
    */
    Tile& getTile(sf::Vector2f position);
    /**
    * @brief Get a Tile
    * @param Int vector2 of position
    * @return Tile of the given coordinate
    */
    Tile& getTile(sf::Vector2i position);

    /**
    * @brief Get an iterator of buildings vector
    * @return Pair of iterator ( begin() and end() )
    */
    std::pair<std::vector<EntityInfo>::iterator,
            std::vector<EntityInfo>::iterator> getBuildingsIt();
    /**
    * @brief Get an iterator of entities vector
    * @return Pair of iterator ( begin() and end() )
    */
    std::pair<std::vector<EntityInfo>::iterator,
            std::vector<EntityInfo>::iterator> getEntitiesIt();

    /**
    * @brief Add a building in the vector
    * @param The build to add
    */
    void addBuildings(EntityInfo build);
    /**
    * @brief Suppress a building of the vector
    * @param The iterator of the building to suppress
    */
    void supBuildings(std::vector<EntityInfo>::iterator it);

    /**
    * @brief Add an entity in the vector
    * @param The entity to add
    */
    void addEntity(EntityInfo entity);
    /**
    * @brief Suppress an entity of the vector
    * @param The iterator of the entity to suppress
    */
    void supEntity(std::vector<EntityInfo>::iterator it);

    /**
    * @brief Get the size of the Tiles
    * @return Size of the tile
    */
    float getBlockSize() const;

    /**
     * @brief Draws the object to the target
     * @param Target Target to render the object to
     * @param states Informations on how to render the object
     */
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

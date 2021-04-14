//
// Created by thibaut on 01/02/2021.
//

#ifndef LIFPROJET_MAPEDITORSTATE_HPP
#define LIFPROJET_MAPEDITORSTATE_HPP

#include "../../Core/GUI/ScrollingContainer.hpp"
#include "../../Core/StateStack/State.hpp"
#include "../../Core/GUI/Container.hpp"
#include "../../Core/GUI/Button.hpp"
#include "../../Core/GUI/CheckBox.hpp"
#include "../../Core/GUI/ButtonTexture.hpp"
#include "../../Game/Map/TilesMap.hpp"

#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Texture.hpp>
#include <SFML/Window/Event.hpp>

namespace Editor {
    enum Tool {
        Standard, Square3, Fill, Eraser, PlaceSoldier, PlaceBuilding,
    };
}

class MapEditorState : public State {
public:
    /**
     * @brief Parameter constructor
     * @param stack State stack where to push the state
     * @param context Context used to access useful structures
     */
    MapEditorState(StateStack& stack, Context context);

    /**
     * @brief Draw the state
     */
    void draw() override;
    /**
     * @brief Update the state
     * @param dt Time interval since the last update
     */
    bool update(sf::Time dt) override;
    /**
     * @brief Handle the events of the state
     * @param event
     */
    bool handleEvent(const sf::Event& event) override;

    /**
     * @brief Paint a 3x3 square from a position
     * @param coordinate Center of the square
     */
    void paintSquare3( sf::Vector2i coordinate);
    /**
     * @brief Fill an entire zone of the same material with another
     * @param coordinate Origin of the fill
     */
    void paintFill( sf::Vector2i coordinate);

    /**
     * @brief Fill a rectangle
     * @param coordinate Origin of the fill
     * @param isPaint If the origin is not blank
     */
    void recPaintFill(sf::Vector2i coordinate, bool* isPaint);

    /**
     * @brief Creates a soldier at the given posititon
     * @param pos Position where to create the soldier
     */
    void createSoldier(sf::Vector2i pos);
    /**
     * @brief Deletes a soldier at the given posititon
     * @param pos Position where to delete the soldier
     */
    bool supressSoldier(sf::Vector2i pos);
    /**
     * @brief Creates a building at the given postiton
     * @param pos Position where to create the building
     */
    void createBuildings(sf::Vector2i pos);
    /**
     * @brief Deletes a building at the given posititon
     * @param pos Position where to delete the building
     */
    bool supressBuildings(sf::Vector2i pos);

    /**
     * @brief Adds a button to the texture scrolling container
     * @param id Id of the button
     * @param pos Position of the button
     */
    void addButtonTexture(sf::Vector2i id, sf::Vector2i pos);
    /**
     * @brief Adds a button to the building scrolling container
     * @param id Id of the button
     * @param pos Position of the button
     */
    void addButtonBuilding(sf::Vector2i id, sf::Vector2i pos);

private :
    sf::Sprite background;
    sf::Sprite subBackground;
    TilesMap map;
    sf::Text mapPath;
    sf::Text editorCoord;
    std::string mMapPath;
    bool subMenu;
    bool saveload; // true : save | false : load

    GUI::Container mEditBar;
    GUI::Container mToolBar;
    GUI::Container mRotationBar;
    GUI::Container mTeamSelection;
    GUI::ScrollingContainer mPaletteBar;
    GUI::ScrollingContainer mBuildingbar;

    GUI::Container mSubMenu;

    sf::Vector2i ground_selection;
    sf::Vector2i lastGround;
    sf::Vector2i lastTileUpdate;
    Editor::Tool tool;
    EntityInfo::ID mEntity;
    EntityInfo::Team mTeam;
    float rotate;

};

#endif //LIFPROJET_MAPEDITORSTATE_HPP

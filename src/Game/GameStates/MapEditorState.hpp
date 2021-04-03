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
    MapEditorState(StateStack& stack, Context context);

    void draw() override;
    bool update(sf::Time dt) override;
    bool handleEvent(const sf::Event& event) override;

    void paintSquare3( sf::Vector2i coordinate);
    void paintFill( sf::Vector2i coordinate);

    void recPaintFill(sf::Vector2i coordinate, bool* isPaint);

    void createSoldier(sf::Vector2i pos);
    bool supressSoldier(sf::Vector2i pos);
    void createBuildings(sf::Vector2i pos);
    bool supressBuildings(sf::Vector2i pos);

    void addButtonTexture(sf::Vector2i id, sf::Vector2i pos);

private :
    sf::Sprite background;
    sf::Sprite subBackground;
    TilesMap map;
    sf::Text mapPath;
    sf::Text editorCoord;
    std::string mMapPath;
    bool subMenu;
    bool saveload; // true : save | false : load

    GUI::Container mSoldierBar;
    GUI::Container mEditBar;
    GUI::Container mToolBar;
    GUI::Container mRotationBar;
    GUI::Container mTeamSelection;
    GUI::ScrollingContainer mPaletteBar;

    GUI::Container mSubMenu;

    float rotate;
    //std::string mMapPath;

    EntityInfo::ID mBuild_selection;
    sf::Vector2i ground_selection;
    sf::Vector2i lastGround;
    sf::Vector2i lastTileUpdate;
    Editor::Tool tool;
    EntityInfo::ID mEntity;
    EntityInfo::Team mTeam;

};



#endif //LIFPROJET_MAPEDITORSTATE_HPP

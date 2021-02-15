//
// Created by thibaut on 01/02/2021.
//

#ifndef LIFPROJET_MAPEDITORSTATE_HPP
#define LIFPROJET_MAPEDITORSTATE_HPP

#include "../../Core/StateStack/State.hpp"
#include "../../Core/GUI/Container.hpp"
#include "../../Core/GUI/Button.hpp"

#include "../../Game/Map/TilesMap.hpp"

#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Texture.hpp>
#include <SFML/Window/Event.hpp>

class MapEditorState : public State {
public:
    MapEditorState(StateStack& stack, Context context);

    virtual void draw();
    virtual bool update(sf::Time dt);
    virtual bool handleEvent(const sf::Event& event);

    void paintSquare3( sf::Vector2i coordinate);
    void paintCircle5( sf::Vector2i coordinate);
    void paintFill( sf::Vector2i coordinate);

    void recPaintFill(sf::Vector2i coordinate, bool* isPaint);

    void createBuildings(sf::Vector2i pos);
    void setBuildings();
    void supressBuildings(sf::Vector2i pos);

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
    GUI::Container mPaletteBar;

    GUI::Container mSubMenu;

    float rotate;
    //std::string mMapPath;

    Textures::building::ID mBuild_selection;
    Textures::ground::ID ground_selection;
    Textures::ground::ID lastGround;
    sf::Vector2i lastTileUpdate;
    map::tool tool;

};



#endif //LIFPROJET_MAPEDITORSTATE_HPP

//
// Created by thibaut on 01/02/2021.
//

#ifndef LIFPROJET_MAPEDITORSTATE_HPP
#define LIFPROJET_MAPEDITORSTATE_HPP

#include "../StateStack/State.hpp"
#include "../Map/TilesMap.hpp"

#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Texture.hpp>
#include <SFML/Window/Event.hpp>

class MapEditorState : public State {
public:
    MapEditorState(StateStack& stack, Context context);

    virtual void draw();
    virtual bool update(sf::Time dt);
    virtual bool handleEvent(const sf::Event& event);

private :
    TilesMap map;

    void switchGround(const sf::Event& event);
};

#endif //LIFPROJET_MAPEDITORSTATE_HPP

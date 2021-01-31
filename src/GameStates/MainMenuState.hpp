//
// Created by 33771 on 31/01/2021.
//

#ifndef LIFPROJET_MAINMENUSTATE_HPP
#define LIFPROJET_MAINMENUSTATE_HPP

#include "../StateStack/State.hpp"

#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Texture.hpp>

class MainMenuState : public State {
public:
    MainMenuState(StateStack& stack, Context context);

    virtual void draw();
    virtual bool update(sf::Time dt);
    virtual bool handleEvent(const sf::Event& event);


private :
    sf::Sprite background;
};


#endif //LIFPROJET_MAINMENUSTATE_HPP

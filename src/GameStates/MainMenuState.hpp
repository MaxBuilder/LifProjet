//
// Created by 33771 on 31/01/2021.
//

#ifndef LIFPROJET_MAINMENUSTATE_HPP
#define LIFPROJET_MAINMENUSTATE_HPP

#include "../StateStack/State.hpp"
#include "../GUI/Container.hpp"
#include "../GUI/Button.hpp"
#include "../Util/Utility.hpp"
#include "../Util/Rand.hpp"

#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Texture.hpp>
#include <SFML/System/Sleep.hpp>

class MainMenuState : public State {
public:
    MainMenuState(StateStack& stack, Context context);

    virtual void draw();
    virtual bool update(sf::Time dt);
    virtual bool handleEvent(const sf::Event& event);


private :
    sf::Sprite background;
    sf::Sprite title;
    GUI::Container mGUIContainer;
};


#endif //LIFPROJET_MAINMENUSTATE_HPP

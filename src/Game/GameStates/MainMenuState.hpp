//
// Created by 33771 on 31/01/2021.
//

#ifndef LIFPROJET_MAINMENUSTATE_HPP
#define LIFPROJET_MAINMENUSTATE_HPP

#include "../../Core/StateStack/State.hpp"
#include "../../Core/GUI/Container.hpp"
#include "../../Core/GUI/Button.hpp"
#include "../../Core/Util/Utility.hpp"
#include "../../Core/Util/Rand.hpp"

#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Texture.hpp>
#include <SFML/System/Sleep.hpp>

class MainMenuState : public State {
public:
    MainMenuState(StateStack& stack, Context context);

    void draw() override;
    bool update(sf::Time dt) override;
    bool handleEvent(const sf::Event& event) override;


private :
    sf::Sprite background;
    sf::Sprite title;
    GUI::Container mGUIContainer;
};


#endif //LIFPROJET_MAINMENUSTATE_HPP

//
// Created by 33771 on 18/02/2021.
//

#ifndef LIFPROJET_PAUSESTATE_HPP
#define LIFPROJET_PAUSESTATE_HPP

#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Text.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Window/Event.hpp>

#include "../../Core/StateStack/State.hpp"
#include "../../Core/Util/Utility.hpp"
#include "../../Core/GUI/Container.hpp"
#include "../../Core/GUI/Button.hpp"

class PauseState : public State {
public:
    PauseState(StateStack& stack, Context& context);

    void draw() override;
    bool update(sf::Time dt) override;
    bool handleEvent(const sf::Event& event) override;

private:
    sf::Text mText;
    sf::Sprite mBackground;
    GUI::Container mButtons;
};


#endif //LIFPROJET_PAUSESTATE_HPP

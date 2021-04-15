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
    /**
     * @brief Parameter constructor
     * @param stack State stack where to push the state
     * @param context Context used to access useful structures
     */
    PauseState(StateStack& stack, Context& context);

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
     * @param event Event to handle
     */
    bool handleEvent(const sf::Event& event) override;

private:
    sf::Text mText;
    sf::Sprite mBackground;
    GUI::Container mButtons;
    sf::View mDefaultView;
};


#endif //LIFPROJET_PAUSESTATE_HPP

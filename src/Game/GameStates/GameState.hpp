//
// Created by 33771 on 14/02/2021.
//

#ifndef LIFPROJET_GAMESTATE_HPP
#define LIFPROJET_GAMESTATE_HPP

#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Window/Event.hpp>
#include <SFML/Graphics/Text.hpp>
#include <SFML/System.hpp>

#include "../../Core/StateStack/State.hpp"
#include "../../Core/Util/Utility.hpp"
#include "../../Core/GUI/Button.hpp"
#include "../../Core/GUI/Container.hpp"
#include "../World/World.hpp"

class GameState : public State {
public:
    GameState(StateStack& stack, Context& context);

    void draw() override;
    bool update(sf::Time dt) override;
    bool handleEvent(const sf::Event& event) override;

private:
    World mWorld;

    GUI::Container mUI;
    sf::Sprite mTimeUI;
    sf::Text mTrackText;
    sf::Text mTimeText;
    sf::Time mTime;

    sf::View mView;
    sf::Vector2f mDirection;
    float mSpeed;
    int mScroll;
    int mTimeSpeed;
    bool mTracking;

};


#endif //LIFPROJET_GAMESTATE_HPP

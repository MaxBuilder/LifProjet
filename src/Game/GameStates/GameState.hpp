//
// Created by 33771 on 14/02/2021.
//

#ifndef LIFPROJET_GAMESTATE_HPP
#define LIFPROJET_GAMESTATE_HPP

#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Window/Event.hpp>

#include "../../Core/StateStack/State.hpp"
#include "../World/World.hpp"

class GameState : public State {
public:
    GameState(StateStack& stack, Context& context);

    void draw() override;
    bool update(sf::Time dt) override;
    bool handleEvent(const sf::Event& event) override;

private:
    World mWorld;

    sf::View mView;
    sf::Vector2f mDirection;
    float mSpeed;
    int mScroll;
};


#endif //LIFPROJET_GAMESTATE_HPP

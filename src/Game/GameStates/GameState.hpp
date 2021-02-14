//
// Created by 33771 on 14/02/2021.
//

#ifndef LIFPROJET_GAMESTATE_HPP
#define LIFPROJET_GAMESTATE_HPP

#include "../../Core/StateStack/State.hpp"

class GameState : public State {
public:
    GameState(StateStack& stack, Context& context);

    virtual void draw();
    virtual bool update(sf::Time dt);
    virtual bool handleEvent(const sf::Event& event);

private:
    //

};


#endif //LIFPROJET_GAMESTATE_HPP

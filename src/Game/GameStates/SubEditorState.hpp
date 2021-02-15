//
// Created by 33771 on 15/02/2021.
//

#ifndef LIFPROJET_SUBEDITORSTATE_HPP
#define LIFPROJET_SUBEDITORSTATE_HPP

#include "../../Core/StateStack/State.hpp"
#include "../../Core/GUI/Container.hpp"
#include "../../Core/GUI/Button.hpp"



class SubEditorState : public State {
public:
    SubEditorState(StateStack& stack, Context context);

    virtual void draw();
    virtual bool update(sf::Time dt);
    virtual bool handleEvent(const sf::Event& event);

private:
    sf::Sprite mBackground;
    GUI::Container mButtons;
};


#endif //LIFPROJET_SUBEDITORSTATE_HPP

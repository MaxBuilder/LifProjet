//
// Created by 33771 on 15/02/2021.
//

#include "SubEditorState.hpp"
#include "MapEditorState.hpp"

SubEditorState::SubEditorState(StateStack &stack, Context context)
: State(stack, context)
, mButtons()

{

}

void SubEditorState::draw() {
    sf::RenderWindow& window = getContext().window;
    window.draw(mBackground);
    window.draw(mButtons);
}

bool SubEditorState::update(sf::Time dt) {
    return true;
}

bool SubEditorState::handleEvent(const sf::Event &event) {
    mButtons.handleEvent(event, getContext().window);
    return false;
}
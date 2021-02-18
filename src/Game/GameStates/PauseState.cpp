//
// Created by 33771 on 18/02/2021.
//

#include "PauseState.hpp"

PauseState::PauseState(StateStack &stack, Context& context)
: State(stack, context)
, mText("Pause", context.fonts.get(Fonts::Main))
, mBackground(context.textures.get(Textures::SubEditorBackground))
{
    mText.setCharacterSize(100u);
    mText.setPosition(640, 200);
    centerOrigin(mText);

    auto returnButton = std::make_shared<GUI::Button>(context, 500, 70, Textures::MenuButton);
    returnButton->setPosition(380, 360);
    returnButton->setText("Return");
    returnButton->setCallback([this] () {
        requestStackPop();
        getContext().sounds.play(Sounds::Menu);
    });
    mButtons.pack(returnButton);

    auto quitButton = std::make_shared<GUI::Button>(context, 500, 70, Textures::MenuButton);
    quitButton->setPosition(380, 460);
    quitButton->setText("Quit");
    quitButton->setCallback([this] () {
        requestStackPop();
        requestStackPop();
        requestStackPush(States::MainMenu);
        getContext().sounds.play(Sounds::Menu);
    });
    mButtons.pack(quitButton);

}

void PauseState::draw() {
    sf::RenderWindow& window = getContext().window;
    sf::View defaultView = window.getDefaultView();
    window.setView(defaultView);

    window.draw(mBackground);
    window.draw(mText);
    window.draw(mButtons);
}

bool PauseState::update(sf::Time dt) {

    return true;
}

bool PauseState::handleEvent(const sf::Event &event) {
    if(sf::Keyboard::isKeyPressed(sf::Keyboard::E))
        requestStackPop();

    mButtons.handleEvent(event, getContext().window);

    return false;
}
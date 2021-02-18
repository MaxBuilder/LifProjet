//
// Created by 33771 on 31/01/2021.
//

#include "MainMenuState.hpp"

#include "../../Game/Resources/ResourceHolder.hpp"

MainMenuState::MainMenuState(StateStack &stack, Context context)
: State(stack, context)
, mGUIContainer() {
    Textures::ID backgroundID;
    switch(Random::Generate(1, 4)) {
        case 1: backgroundID = Textures::MenuBackground1; break;
        case 2: backgroundID = Textures::MenuBackground2; break;
        case 3: backgroundID = Textures::MenuBackground3; break;
        case 4: backgroundID = Textures::MenuBackground4; break;
    }

    sf::Texture& backgroundTexture = context.textures.get(backgroundID);
    background.setTexture(backgroundTexture);
    sf::Texture& titleTexture = context.textures.get(Textures::MenuTitle);
    title.setTexture(titleTexture);
    centerOrigin(title);
    title.setPosition(640, 180);

    auto playButton = std::make_shared<GUI::Button>(context, 500, 70, Textures::MenuButton);
    playButton->setPosition(390, 360);
    playButton->setText("Simulation");
    playButton->setCallback([this] () {
        requestStackPop();
        requestStackPush(States::Game);
        getContext().sounds.play(Sounds::Menu);
    });

    auto editorButton = std::make_shared<GUI::Button>(context, 500, 70, Textures::MenuButton);
    editorButton->setPosition(390, 460);
    editorButton->setText("Edit MapGround");
    editorButton->setCallback([this] () {
        requestStackPop();
        requestStackPush(States::MapEditor);
        getContext().sounds.play(Sounds::Menu);
    });

    auto quitButton = std::make_shared<GUI::Button>(context, 500, 70, Textures::MenuButton);
    quitButton->setPosition(390, 560);
    quitButton->setText("Quit");
    quitButton->setCallback([this] () {
        requestStackPop();
        getContext().sounds.play(Sounds::Menu);
        sf::sleep(sf::milliseconds(200));
    });

    mGUIContainer.pack(playButton);
    mGUIContainer.pack(editorButton);
    mGUIContainer.pack(quitButton);
}

void MainMenuState::draw() {
    sf::RenderWindow& window = getContext().window;
    window.draw(background);
    window.draw(title);
    window.draw(mGUIContainer);
}

bool MainMenuState::update(sf::Time dt) {
    return true;
}

bool MainMenuState::handleEvent(const sf::Event &event) {
    mGUIContainer.handleEvent(event, getContext().window);
    return false;
}
//
// Created by thibaut on 15/02/2021.
//

#ifndef LIFPROJET_SETTINGSSTATE_HPP
#define LIFPROJET_SETTINGSSTATE_HPP

#include "../../Core/StateStack/State.hpp"
#include "../../Core/GUI/Container.hpp"
#include "../../Core/GUI/CheckBox.hpp"
#include "../../Core/GUI/ButtonList.hpp"
#include "../../Core/GUI/Button.hpp"
#include "../../Core/Util/Utility.hpp"

#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Texture.hpp>
#include <SFML/System/Sleep.hpp>
#include <fstream>
#include <iostream>

class SettingsState : public State {
public:
    SettingsState(StateStack& stack, Context context);
    ~SettingsState() override;

    void draw() override;
    bool update(sf::Time dt) override;
    bool handleEvent(const sf::Event& event) override;

    void saveSettings();
    void apply();

private :
    sf::Sprite mBackground;
    GUI::Container mGUIContainer;
    GUI::Container mButtonList;
    GUI::Container mCheckBox;

    sf::Text mTextWindowSize;
    sf::Text mTextFullscreen;



    int mWidth, mHeight;
    int mWindowStyle;


};

#endif //LIFPROJET_SETTINGSSTATE_HPP

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

#include <iostream>
#include <fstream>

class SettingsState : public State {
public:
    /**
     * @brief Parameter constructor
     * @param stack State stack where to push the state
     * @param context Context used to access useful structures
     */
    SettingsState(StateStack& stack, Context context);
    /**
     * @brief Destructor
     */
    ~SettingsState() override;

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

    /**
     * @brief Saves the settings to a file
     */
    void saveSettings() const;
    /**
     * @brief Applies the modification of the settings
     */
    void apply();

private :
    sf::Sprite mBackground;
    GUI::Container mGUIContainer;
    GUI::Container mResolutionList;
    GUI::Container mVolumeList;
    GUI::Container mCheckBox;

    sf::Text mTextWindowSize;
    sf::Text mTextFullscreen;
    sf::Text mTextVolume;

    int mWidth, mHeight;
    int mWindowStyle;
    int mVolume;


};

#endif //LIFPROJET_SETTINGSSTATE_HPP

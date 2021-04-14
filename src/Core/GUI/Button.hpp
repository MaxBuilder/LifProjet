//
// Created by 33771 on 01/02/2021.
//

#ifndef LIFPROJET_BUTTON_HPP
#define LIFPROJET_BUTTON_HPP

#include "Component.hpp"
#include "../../Core/StateStack/State.hpp"
#include "../../Core/Util/Utility.hpp"
#include "../../Game/Resources/ResourceIdentifiers.hpp"

#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Text.hpp>

#include <functional>
#include <memory>
#include <string>

namespace GUI {

    /**
     * @class Button
     * @brief GUI element representing a button
     * @authors SFML Game Development (base) heavily modified to integrate custom sizes, textures and mouse clicking
     */
    class Button : public Component {
    public:
        typedef std::shared_ptr<Button> Ptr;
        typedef std::function<void()> Callback;

        /**
         * @enum ButtonState
         * @brief Describes the state of the button
         */
        enum ButtonState {
            Normal,
            Pressed,
            Selected,
            ButtonCount
        };

    public:
        /**
         * @brief Parameter constructor
         * @param context Context
         * @param width Width of the button
         * @param height Height of the button
         * @param textureID Identifier of the texture the button will have
         */
        Button(State::Context context, float width, float height, Textures::ID textureID);

        /**
         * @brief Setter of the button's callback
         * @param callback Callback
         */
        virtual void setCallback(Callback callback);
        /**
         * @brief Sets the text of the button
         * @param text Text to assign
         */
        void setText(const std::string& text);

        /**
         * @brief Sets if the button can be toggled
         * @param flag Flag
         */
        void setToggle(bool flag);
        /**
         * @brief Getter of the button toggle
         * @return true if the button can be toggled
         */
        bool getToggle();

        /**
         * @brief Returns if the button is selectable
         * @return true if the button is selectable
         */
        bool isSelectable() const override;
        /**
         * @brief Select the button
         */
        void select() override;
        /**
         * @brief Deselects the button
         */
        void deselect() override;

        /**
         * @brief Activates the button
         */
        void activate() override;
        /**
         * @brief Deactivates the button
         */
        void deactivate() override;

        /**
         * @brief Handles the events
         * @param event Event to process
         */
        void handleEvent(const sf::Event& event) override;

    private:
        /**
         * @brief Draws the object to the target
         * @param target Target to render the object to
         * @param states Information on how to render the object
         */
        void draw(sf::RenderTarget& target, sf::RenderStates states) const override;

        /**
         * @brief Changes the texture relative to the state of the button
         * @param buttonState New state of the button
         */
        void changeTexture(ButtonState buttonState);

    protected:
        Callback mCallback;
        sf::Sprite mSprite;
        sf::Text mText;
        bool mIsToggle;
    };

}



#endif //LIFPROJET_BUTTON_HPP

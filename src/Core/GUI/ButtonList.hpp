//
// Created by thibaut on 15/02/2021.
//

#ifndef LIFPROJET_BUTTONLIST_HPP
#define LIFPROJET_BUTTONLIST_HPP

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
     * @class ButtonList
     * @brief Button capable of switching between several options
     */
    class ButtonList : public Component {
    public:
        typedef std::shared_ptr<ButtonList> Ptr;
        typedef std::function<void(int count)> Callback;

        /**
         * @brief Parameter constructor
         * @param context Context
         * @param width Width of the button
         * @param height Height of the button
         * @param characterSize Size of the text's characters
         * @param textureID Identifier of the texture the button will have
         */
        ButtonList(State::Context context, float width, float height, int characterSize, Textures::ID textureID = Textures::None);

        /**
         * @brief Setter of the button's callback
         * @param callback Callback
         */
        void setCallback(Callback callback);

        /**
         * @brief Add an option to the list
         * @param strOption String to be displayed when selected
         */
        void addOption(const std::string& strOption);
        /**
         * @brief Sets option at specified index
         * @param index Index to set the option to
         */
        void setOption(int index);

        /**
         * @brief Changes current option to the next
         */
        void nextOption();
        /**
         * @brief Changes current option to the previous
         */
        void previousOption();

        /**
         * Getter of the button selection
         * @return true if the button is selectable
         */
        bool isSelectable() const override;
        /**
         * @brief Handles events
         * @param event Event to process
         */
        void handleEvent(const sf::Event& event) override;
        /**
         * @brief Handles mouse events
         * @param event Event to process
         * @param window Window to handle mouse events
         */
        void handleEvent(const sf::Event& event,const sf::RenderWindow& window);


    private:
        /**
         * Draws the object to the target
         * @param target Target to render the object to
         * @param states Information on how to render the object
         */
        void draw(sf::RenderTarget& target, sf::RenderStates states) const override;

        Callback mCallback;
        sf::Sprite mSprite;
        sf::Text mTextInf,mTextSup,mText;
        std::vector<std::string> mOptions;

        int mCount;
    };

}

#endif //LIFPROJET_BUTTONLIST_HPP

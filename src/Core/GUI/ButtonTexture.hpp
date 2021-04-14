//
// Created by thibaut on 01/03/2021.
//

#ifndef LIFPROJET_BUTTONTEXTURE_HPP
#define LIFPROJET_BUTTONTEXTURE_HPP

#include "Button.hpp"
namespace GUI {

    /**
     * @class ButtonTexture
     * @brief Button used in the editor to select the textures
     */
    class ButtonTexture : public GUI::Button {

    public:
        typedef std::shared_ptr<ButtonTexture> Ptr;
        typedef std::function<void(sf::Vector2i bId)> CallbackId;

        /**
         * @brief Parameter constructor
         * @param context Context
         * @param width Width of the button
         * @param height Height of the button
         * @param textureID Identifier of the texture the button will have
         * @param id Id of the button in the list
         */
        ButtonTexture(State::Context context, float width, float height, Textures::ID textureID, sf::Vector2i id);
        /**
         * @brief Activates the button
         */
        void activate() override;
        /**
         * @brief Deactivates the button
         */
        void deactivate() override;
        /**
         * @brief Setter of the button's callback
         * @param callback Callback
         */
        virtual void setCallback(CallbackId callback);

    private:
        sf::Vector2i bId;
        CallbackId mCallbackId;

    };
}

#endif //LIFPROJET_BUTTONTEXTURE_HPP

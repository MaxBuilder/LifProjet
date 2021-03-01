//
// Created by thibaut on 01/03/2021.
//

#ifndef LIFPROJET_BUTTONTEXTURE_HPP
#define LIFPROJET_BUTTONTEXTURE_HPP

#include "Button.hpp"
namespace GUI {

    class ButtonTexture : public GUI::Button {

    public:
        typedef std::shared_ptr<ButtonTexture> Ptr;
        typedef std::function<void(sf::Vector2i bId)> CallbackId;

        ButtonTexture(State::Context context, float width, float height, Textures::ID textureID, sf::Vector2i id);
        void activate();
        void deactivate();
        virtual void setCallback(CallbackId callback);

    private:
        sf::Vector2i bId;
        CallbackId mCallbackId;

    };
}

#endif //LIFPROJET_BUTTONTEXTURE_HPP

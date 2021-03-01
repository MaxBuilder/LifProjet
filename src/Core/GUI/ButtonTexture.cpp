//
// Created by thibaut on 01/03/2021.
//

#include "ButtonTexture.hpp"
namespace GUI{

    ButtonTexture::ButtonTexture(State::Context context, float width, float height, Textures::ID textureID,
                                 sf::Vector2i id) : Button (context,  width,  height, textureID) {

        mSprite.setTextureRect(sf::IntRect (16*id.x,16*id.y,16,16));
        bId = id;
        mSprite.setScale(width/16.f,height/16.f);
    }

    void ButtonTexture::setCallback(CallbackId callback){
        mCallbackId = std::move(callback);
    }

    void ButtonTexture::activate(){
        if(mCallbackId)
            mCallbackId(bId);
    }

    void ButtonTexture::deactivate(){

    }
}
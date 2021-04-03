//
// Created by thibaut on 19/02/2021.
//

#include "CheckBox.hpp"

namespace GUI {

    CheckBox::CheckBox(State::Context context, float width, float height, Textures::ID textureID)
            : mCallback()
            , mSprite(context.textures.get(textureID))
            , mIsToggle(false){

        mHeight = height;
        mWidth = width;
        changeTexture(Normal);
    }

    void CheckBox::setCallback(Callback callback) {
        mCallback = std::move(callback);
    }

    void CheckBox::setToggle(bool flag) {
        mIsToggle = flag;
    }

    bool CheckBox::getToggle(){
        return mIsToggle;
    }

    bool CheckBox::isSelectable() const {
        return false;
    }

    void CheckBox::activate() {
        if (Component::isActive()){
            Component::deactivate();
            changeTexture(Normal);
        }
        else{
            Component::activate();
            changeTexture(Pressed);
        }
        if(mCallback)
            mCallback(Component::isActive());

    }

    void CheckBox::deactivate() {
        Component::deactivate();
    }

    void CheckBox::handleEvent(const sf::Event& event) {}

    void CheckBox::draw(sf::RenderTarget &target, sf::RenderStates states) const {
        states.transform *= getTransform();
        target.draw(mSprite, states);
    }

    void CheckBox::changeTexture(Type buttonType) {
        sf::IntRect textureRect(0, mWidth * buttonType, mWidth, mHeight);
        mSprite.setTextureRect(textureRect);
    }

}
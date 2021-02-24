//
// Created by 33771 on 01/02/2021.
//

#include "Button.hpp"

#include "../../Game/Resources/ResourceHolder.hpp"

namespace GUI {

    Button::Button(State::Context context, float width, float height, Textures::ID textureID)
    : mCallback()
    , mSprite(context.textures.get(textureID))
    , mText("", context.fonts.get(Fonts::Main), 16)
    , mIsToggle(false){
        mHeight = height;
        mWidth = width;
        changeTexture(Normal);

        sf::FloatRect bounds = mSprite.getLocalBounds();
        mText.setPosition(bounds.width / 2.f, bounds.height / 2.f);
    }

    void Button::setCallback(Callback callback) {
        mCallback = std::move(callback);
    }

    void Button::setText(const std::string &text) {
        mText.setString(text);
        centerOrigin(mText);
    }

    void Button::setToggle(bool flag) {
        mIsToggle = flag;
    }

    bool Button::getToggle(){
        return mIsToggle;
    }

   bool Button::isSelectable() const {
        return false;
    }

    void Button::select() {
        Component::select();
        changeTexture(Selected);
    }

    void Button::deselect() {
        Component::deselect();
    }

    void Button::activate() {
        Component::activate();

        if(mIsToggle)
            changeTexture(Pressed);

        if(mCallback)
            mCallback();

        if(!mIsToggle)
            deactivate();
    }

    void Button::deactivate() {
        Component::deactivate();

        if(mIsToggle) {
            if(isSelected())
                changeTexture(Selected);
            else changeTexture(Normal);
        }
    }

    void Button::handleEvent(const sf::Event& event) {}

    void Button::draw(sf::RenderTarget &target, sf::RenderStates states) const {
        states.transform *= getTransform();
        target.draw(mSprite, states);
        target.draw(mText, states);
    }

    void Button::changeTexture(Type buttonType) {
        sf::IntRect textureRect(0, mHeight * buttonType, mWidth, mHeight);
        mSprite.setTextureRect(textureRect);
    }

}
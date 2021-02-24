//
// Created by thibaut on 15/02/2021.
//

#include "ButtonList.hpp"

namespace GUI {

    ButtonList::ButtonList(State::Context context, float width, float height, int characterSize,
                                Textures::ID textureID) :
            mSprite(context.textures.get(textureID)),
            mCount(-1){

        mHeight = height;
        mWidth = width;

        mTextInf.setCharacterSize(characterSize);
        mTextSup.setCharacterSize(characterSize);
        mText.setCharacterSize(characterSize);

        mTextInf.setString("<<");
        mTextSup.setString(">>");
        mText.setString("");

        mTextInf.setFont(context.fonts.get(Fonts::Main));
        mTextSup.setFont(context.fonts.get(Fonts::Main));
        mText.setFont(context.fonts.get(Fonts::Main));

        mTextInf.setPosition(0, height / 2.f);
        mTextSup.setPosition(mWidth-mTextSup.getLocalBounds().width, height / 2.f);
        mText.setPosition((mWidth-mText.getLocalBounds().width) / 2.f, height / 2.f);

    }

    void ButtonList::setCallback(Callback callback) {
        mCallback = std::move(callback);
    }

    void ButtonList::setOption(int index){
        mCount = index;
        mText.setString(mOptions.at(mCount));
        mText.setPosition((mWidth-mText.getLocalBounds().width) / 2.f, mHeight / 2.f);
    }

    void ButtonList::addOption(const std::string& strOption) {
        mCount++;
        mOptions.push_back(strOption);
        mText.setString(strOption);
        mText.setPosition((mWidth-mText.getLocalBounds().width) / 2.f, mHeight / 2.f);
    }

    void ButtonList::nextOption() {
        if (mCount >= mOptions.size() - 1) return;
        mCount++;
        mText.setString(mOptions.at(mCount));
        mText.setPosition((mWidth-mText.getLocalBounds().width) / 2.f, mHeight / 2.f);
        mCallback(mCount);
    }

    void ButtonList::previousOption() {
        if (mCount <= 0) return;
        mCount--;
        mText.setString(mOptions.at(mCount));
        mText.setPosition((mWidth-mText.getLocalBounds().width) / 2.f, mHeight / 2.f);
        mCallback(mCount);
    }

    void ButtonList::handleEvent(const sf::Event &event, const sf::RenderWindow &window) {
        sf::Vector2i mouseCoord = sf::Mouse::getPosition(window);
        mouseCoord = static_cast<sf::Vector2i>(window.mapPixelToCoords(mouseCoord));

        if (mouseCoord.x >= getPosition().x and mouseCoord.x <= getPosition().x + getWidth() / 2.f
            and mouseCoord.y >= getPosition().y and mouseCoord.y <= getPosition().y + getHeight())
            previousOption();
        else if (mouseCoord.x >= getPosition().x + getWidth() / 2.f and mouseCoord.x <= getPosition().x + getWidth()
                 and mouseCoord.y >= getPosition().y and mouseCoord.y <= getPosition().y + getHeight())
            nextOption();

    }

    void ButtonList::draw(sf::RenderTarget &target, sf::RenderStates states) const {
        states.transform *= getTransform();

        target.draw(mText, states);
        target.draw(mTextInf, states);
        target.draw(mTextSup, states);
    }

    bool ButtonList::isSelectable() const{
        return true;
    }
    void ButtonList::handleEvent(const sf::Event& event){
        return;
    }

}
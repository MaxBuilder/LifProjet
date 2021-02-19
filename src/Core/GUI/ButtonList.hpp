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

    class ButtonList : public Component {
    public:
        typedef std::shared_ptr<ButtonList> Ptr;
        typedef std::function<void(int count)> Callback;

        ButtonList(State::Context context, float width, float height, int characterSize, Textures::ID textureID = Textures::None);

        void setCallback(Callback callback);

        void addOption(const std::string& strOption);
        void setOption(int index);

        void nextOption();
        void previousOption();

        float getWidth() const;
        float getHeight() const;

        virtual bool isSelectable() const;
        virtual void handleEvent(const sf::Event& event);
        void handleEvent(const sf::Event& event,const sf::RenderWindow& window);


    private:
        void draw(sf::RenderTarget& target, sf::RenderStates states) const override;

        Callback mCallback;
        sf::Sprite mSprite;
        sf::Text mTextInf,mTextSup,mText;
        std::vector<std::string> mOptions;

        int mCount;
        float width, height;
    };

}

#endif //LIFPROJET_BUTTONLIST_HPP

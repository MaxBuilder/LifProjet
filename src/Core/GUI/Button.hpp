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

    class Button : public Component {
    public:
        typedef std::shared_ptr<Button> Ptr;
        typedef std::function<void()> Callback;

        enum Type {
            Normal,
            Pressed,
            Selected,
            ButtonCount
        };

    public:
        Button(State::Context context, float width, float height, Textures::ID textureID);

        virtual void setCallback(Callback callback);
        void setText(const std::string& text);
        void setToggle(bool flag);

        bool getToggle();
        virtual bool isSelectable() const;
        virtual void select();
        virtual void deselect();

        virtual void activate();
        virtual void deactivate();

        virtual void handleEvent(const sf::Event& event);

    private:
        virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;
        void changeTexture(Type buttonType);

    protected:
        Callback mCallback;
        sf::Sprite mSprite;
        sf::Text mText;
        bool mIsToggle;
    };

}



#endif //LIFPROJET_BUTTON_HPP

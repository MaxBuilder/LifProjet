//
// Created by thibaut on 19/02/2021.
//

#ifndef LIFPROJET_CHECKBOX_HPP
#define LIFPROJET_CHECKBOX_HPP

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

    class CheckBox : public Component {
    public:
        typedef std::shared_ptr<CheckBox> Ptr;
        typedef std::function<void(bool check)> Callback;

        enum Type {
            Normal,
            Pressed
        };

    public:
        CheckBox(State::Context context, float width, float height, Textures::ID textureID);

        void setCallback(Callback callback);

        void setToggle(bool flag);
        bool getToggle();

        virtual bool isSelectable() const;

        virtual void activate();
        virtual void deactivate();

        virtual void handleEvent(const sf::Event &event);

    private:
        virtual void draw(sf::RenderTarget &target, sf::RenderStates states) const;

        void changeTexture(Type buttonType);

    private:
        Callback mCallback;
        sf::Sprite mSprite;
        bool mIsToggle;
    };


}

#endif //LIFPROJET_CHECKBOX_HPP

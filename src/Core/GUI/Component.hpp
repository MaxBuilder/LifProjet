//
// Created by 33771 on 01/02/2021.
//

#ifndef LIFPROJET_COMPONENT_HPP
#define LIFPROJET_COMPONENT_HPP

#include <SFML/Graphics/Drawable.hpp>
#include <SFML/Graphics/Transformable.hpp>
#include <SFML/System/NonCopyable.hpp>
#include <SFML/Window/Event.hpp>

#include <memory>

namespace GUI {

    class Component : public sf::Drawable, public sf::Transformable, private sf::NonCopyable {
    public:
        typedef std::shared_ptr<Component> Ptr;

        Component();
        ~Component() override;

        virtual bool isSelectable() const = 0;
        bool isSelected() const;
        virtual void select();
        virtual void deselect();

        virtual bool isActive() const;
        virtual void activate();
        virtual void deactivate();

        float getWidth() const;
        float getHeight() const;

        virtual void handleEvent(const sf::Event& event) = 0;


    private:
        bool mIsSelected;
        bool mIsActive;

    protected:
        float mWidth, mHeight;

    };

}

#endif //LIFPROJET_COMPONENT_HPP

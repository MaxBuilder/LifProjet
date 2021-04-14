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

    /**
     * @class Component
     * @brief Base class of the GUI hierarchy
     * @authors SFML Game Development, modified to integrate different sizes
     */
    class Component : public sf::Drawable, public sf::Transformable, private sf::NonCopyable {
    public:
        typedef std::shared_ptr<Component> Ptr;

        /**
         * @brief Default constructor
         */
        Component();
        /**
         * @brief Virtual constructor (if needed)
         */
        ~Component() override;

        /**
         * @brief Returns if the button is selectable
         * @return true if the button is selectable
         */
        virtual bool isSelectable() const = 0;
        /**
         * @brief Returns if the button is selected
         * @return true if button is selected
         */
        bool isSelected() const;
        /**
         * @brief Select the button
         */
        virtual void select();
        /**
         * @brief Deselects the button
         */
        virtual void deselect();

        /**
         * @brief Returns if the button is selected
         * @return true if the button is selected
         */
        virtual bool isActive() const;
        /**
         * @brief Activates the button
         */
        virtual void activate();
        /**
         * @brief Deactivates the button
         */
        virtual void deactivate();

        /**
         * @brief Returns button's width
         * @return Width
         */
        float getWidth() const;
        /**
         * @brief Returns button's height
         * @return Height
         */
        float getHeight() const;

        /**
         * @brief Handles the events
         * @param event Event to process
         */
        virtual void handleEvent(const sf::Event& event) = 0;


    private:
        bool mIsSelected;
        bool mIsActive;

    protected:
        float mWidth, mHeight;

    };

}

#endif //LIFPROJET_COMPONENT_HPP

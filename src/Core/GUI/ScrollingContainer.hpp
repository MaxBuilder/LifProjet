//
// Created by thibaut on 24/02/2021.
//

#ifndef LIFPROJET_SCROLLINGCONTAINER_HPP
#define LIFPROJET_SCROLLINGCONTAINER_HPP

#include "Container.hpp"

namespace GUI {

    /**
     * @class ScrollingContainer
     * @brief Container used to store large amounts of elements, allows scrolling
     */
    class ScrollingContainer : public Container {
    public:
        typedef std::shared_ptr<ScrollingContainer> Ptr;

        /**
         * @brief Parameter constructor
         * @param rect Bounds of the container
         * @param scrollSpeed Speed at which the elements are scrolled
         */
        ScrollingContainer(sf::IntRect rect, float scrollSpeed);

        /**
         * Handles mouse events of the container
         * @param event Event to process
         * @param window Window to handle mouse events
         */
        void handleEvent(const sf::Event& event, const sf::RenderWindow& window) override;
        /**
         * @brief Adds a component to the structure
         * @param component Component to add
         */
        void pack(Component::Ptr component) override;

    private:
        /**
         * Draws the object to the target
         * @param target Target to render the object to
         * @param states Informations on how to render the object
         */
        void draw(sf::RenderTarget &target, sf::RenderStates states) const override;

    private:
        float scroll;
        float scrollLimit;
        float mScrollSpeed;
        sf::IntRect bounds;
    };

}

#endif //LIFPROJET_SCROLLINGCONTAINER_HPP

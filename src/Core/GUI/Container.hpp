//
// Created by 33771 on 01/02/2021.
//

#ifndef LIFPROJET_CONTAINER_HPP
#define LIFPROJET_CONTAINER_HPP

#include "Component.hpp"

#include <SFML/Window/Event.hpp>
#include <SFML/Graphics/RenderStates.hpp>
#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Graphics/RenderWindow.hpp>

#include <memory>
#include <vector>

namespace GUI {

    /**
     * @class Container
     * @brief Contains components
     * @authors SFML Game Development (base) modified to fit new needs (mouse events and different button types)
     */
    class Container : public Component {
    public:
        typedef std::shared_ptr<Container> Ptr;

        /**
         * @brief Default constructor
         */
        Container();

        /**
         * @brief Adds a component to the structure
         * @param component Component to add
         */
         virtual void pack(Component::Ptr component);

        /**
         * @brief Returns if the button is selectable
         * @return true if the button is selectable
         */
        bool isSelectable() const override;
        /**
         * @brief Handles the events
         * @param event Event to process
         */
        void handleEvent(const sf::Event& event) override;
        /**
         * @brief Handles buttons mouse events
         * @param event Event to process
         * @param window Window to handle mouse events
         */
        virtual void handleEvent(const sf::Event& event, const sf::RenderWindow& window); // Reserved for buttons
        /**
         * @brief Handles list mouse events
         * @param event Event to process
         * @param window Window to handle mouse events
         */
        virtual void handleListEvent(const sf::Event &event, const sf::RenderWindow& window); // Reserved for ButtonList
        /**
         * @brief Handles checkbox mouse events
         * @param event Event to process
         * @param window Window to handle mouse events
         */
        virtual void handleCheckBoxEvent(const sf::Event &event, const sf::RenderWindow& window); // Reserved for Checkbox

    private:
        // Selection functions (never used)
        /**
         * @brief Draws the container to the target
         * @param target Target to render the object to
         * @param states Informations on how to render the object
         */
        void draw(sf::RenderTarget &target, sf::RenderStates states) const override;

        /**
         * @brief Returns if container handles selection
         * @return true if container handles selection
         */
        bool hasSelection() const;
        /**
         * @brief Selects a member of the container at specified index
         * @param index Component to select index
         */
        void select(std::size_t index);
        /**
         * @brief Select next element
         */
        void selectNext();
        /**
         * @brief Select previous element
         */
        void selectPrevious();

    protected:
        std::vector<Component::Ptr> mChildren;
        int mSelectedChild;

    };

}



#endif //LIFPROJET_CONTAINER_HPP

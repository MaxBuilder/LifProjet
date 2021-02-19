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

    class Container : public Component {
    public:
        typedef std::shared_ptr<Container> Ptr;

        Container();

        void pack(Component::Ptr component);

        virtual bool isSelectable() const;
        virtual void handleEvent(const sf::Event& event);
        virtual void handleEvent(const sf::Event& event, const sf::RenderWindow& window); // Reserved for buttons
        virtual void handleListEvent(const sf::Event &event, const sf::RenderWindow& window); // reserve for ButtonList
        virtual void handleCheckBoxEvent(const sf::Event &event, const sf::RenderWindow& window); // reserve for Checkbox

    private:
        virtual void draw(sf::RenderTarget &target, sf::RenderStates states) const;

        bool hasSelection() const;
        void select(std::size_t index);
        void selectNext();
        void selectPrevious();

    private:
        std::vector<Component::Ptr> mChildren;
        int mSelectedChild;

    };

}



#endif //LIFPROJET_CONTAINER_HPP

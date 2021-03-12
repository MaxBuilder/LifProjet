//
// Created by thibaut on 24/02/2021.
//

#include "../../Core/Util/Foreach.hpp"

#include "ScrollingContainer.hpp"
#include "Button.hpp"
#include <iostream>

namespace GUI{

    ScrollingContainer::ScrollingContainer(sf::IntRect rect,float scrollSpeed){
        bounds = rect;
        mScrollSpeed = scrollSpeed;
        scroll = 0;
        scrollLimit = 0;
    }

    void ScrollingContainer::handleEvent(const sf::Event& event, const sf::RenderWindow& window){

        sf::Vector2i mouseCoord = sf::Mouse::getPosition(window);
        mouseCoord = static_cast<sf::Vector2i>(window.mapPixelToCoords(mouseCoord));

        if(mouseCoord.x >= bounds.left and mouseCoord.x <= bounds.left + bounds.width
           and mouseCoord.y >= bounds.top and mouseCoord.y <= bounds.top+bounds.height){

            if (event.type == sf::Event::MouseWheelMoved)
            {
                scroll += static_cast<float>(event.mouseWheel.delta*(mChildren.at(0)->getHeight()+mScrollSpeed));
                if (scroll > 0)
                    scroll = 0;
                if (scroll < -scrollLimit)
                    scroll = -scrollLimit;
            }

            if(event.type == sf::Event::MouseButtonPressed and sf::Mouse::isButtonPressed(sf::Mouse::Left)) {

                float x = mouseCoord.x, y = mouseCoord.y - scroll;
                for (auto &child : mChildren) {
                    std::shared_ptr<Button> derived = std::dynamic_pointer_cast<Button>(child);
                    if (x >= derived->getPosition().x and x <= derived->getPosition().x + derived->getWidth()
                        and y >= derived->getPosition().y and y <= derived->getPosition().y + derived->getHeight()) {

                        if (derived->getToggle()) {
                            for (auto &child2 : mChildren) {
                                std::shared_ptr<Button> derived2 = std::dynamic_pointer_cast<Button>(child2);
                                derived2->deactivate();
                            }
                        }
                        derived->activate();
                    }
                }
            }
        }
    }

    void ScrollingContainer::pack(Component::Ptr component) {
        mChildren.push_back(component);
        float dist = component->getPosition().y+component->getHeight() - bounds.top - bounds.height;
        if (dist > scrollLimit)
            scrollLimit = dist;
    }

    void ScrollingContainer::draw(sf::RenderTarget &target, sf::RenderStates states) const {
        states.transform *= getTransform();
        states.transform.translate(0,scroll);

        FOREACH(const Component::Ptr &child, mChildren){
            if(states.transform.transformPoint(child->getPosition()).y >= bounds.top
               and states.transform.transformPoint(child->getPosition()).y+child->getHeight() <= bounds.top+bounds.height)
                target.draw(*child, states);
        }
    }

}
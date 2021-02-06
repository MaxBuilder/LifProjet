//
// Created by 33771 on 01/02/2021.
//

#include "Container.hpp"
#include <iostream>

#include "../Util/Foreach.hpp"
#include "Button.hpp"

namespace GUI {

    Container::Container()
    : mChildren()
    , mSelectedChild(-1) {
    }

    void Container::pack(Component::Ptr component) {
        mChildren.push_back(component);

        /*if (!hasSelection() && component->isSelectable())
            select(mChildren.size() - 1);*/
    }

    bool Container::isSelectable() const {
        return false;
    }

    void Container::handleEvent(const sf::Event &event) {
        // If we have selected a child then give it events
        if (hasSelection() && mChildren[mSelectedChild]->isActive()) {
            mChildren[mSelectedChild]->handleEvent(event);
        } else if (event.type == sf::Event::KeyReleased) {
            if (event.key.code == sf::Keyboard::W || event.key.code == sf::Keyboard::Up) {
                selectPrevious();
            } else if (event.key.code == sf::Keyboard::S || event.key.code == sf::Keyboard::Down) {
                selectNext();
            } else if (event.key.code == sf::Keyboard::Return || event.key.code == sf::Keyboard::Space) {
                if (hasSelection())
                    mChildren[mSelectedChild]->activate();
            }
        }
    }

    // Reserved for buttons (click action)
    void Container::handleEvent(const sf::Event &event, const sf::RenderWindow& window) {
        sf::Vector2i mouseCoord = sf::Mouse::getPosition(window);
        float x = mouseCoord.x, y = mouseCoord.y;
            for(auto& child : mChildren) {
            std::shared_ptr<Button> derived = std::dynamic_pointer_cast<Button>(child);
            if(x >= derived->getPosition().x and x <= derived->getPosition().x + derived->getWidth()
            and y >= derived->getPosition().y and y <= derived->getPosition().y + derived->getHeight()) {
                if(sf::Mouse::isButtonPressed(sf::Mouse::Left))
                    derived->activate();
            }
        }
    }

    void Container::draw(sf::RenderTarget &target, sf::RenderStates states) const {
        states.transform *= getTransform();

        FOREACH(const Component::Ptr &child, mChildren)target.draw(*child, states);
    }

    bool Container::hasSelection() const {
        return mSelectedChild >= 0;
    }

    void Container::select(std::size_t index) {
        if (mChildren[index]->isSelectable()) {
            if (hasSelection())
                mChildren[mSelectedChild]->deselect();

            mChildren[index]->select();
            mSelectedChild = index;
        }
    }

    void Container::selectNext() {
        if (!hasSelection())
            return;

        // Search next component that is selectable, wrap around if necessary
        int next = mSelectedChild;
        do
            next = (next + 1) % mChildren.size();
        while (!mChildren[next]->isSelectable());

        // Select that component
        select(next);
    }

    void Container::selectPrevious() {
        if (!hasSelection())
            return;

        // Search previous component that is selectable, wrap around if necessary
        int prev = mSelectedChild;
        do
            prev = (prev + mChildren.size() - 1) % mChildren.size();
        while (!mChildren[prev]->isSelectable());

        // Select that component
        select(prev);
    }

}
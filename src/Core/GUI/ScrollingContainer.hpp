//
// Created by thibaut on 24/02/2021.
//

#ifndef LIFPROJET_SCROLLINGCONTAINER_HPP
#define LIFPROJET_SCROLLINGCONTAINER_HPP

#include "Container.hpp"

namespace GUI{

class ScrollingContainer : public Container{
public:
    ScrollingContainer(sf::IntRect rect,float scrollSpeed);

    typedef std::shared_ptr<ScrollingContainer> Ptr;
    virtual void handleEvent(const sf::Event& event, const sf::RenderWindow& window);
    virtual void pack(Component::Ptr component);

private:
    virtual void draw(sf::RenderTarget &target, sf::RenderStates states) const;
    float scroll;
    float scrollLimit;
    float mScrollSpeed;
    sf::IntRect bounds;
};

}

#endif //LIFPROJET_SCROLLINGCONTAINER_HPP

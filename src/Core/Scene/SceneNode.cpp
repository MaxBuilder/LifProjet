//
// Created by 33771 on 13/02/2021.
//

#include "SceneNode.hpp"
#include "../Util/Foreach.hpp"

#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/RenderTarget.hpp>

#include <algorithm>
#include <cassert>
#include <iostream>

SceneNode::SceneNode()
: mChildren()
, mParent(nullptr) {

}

void SceneNode::attachChild(Ptr child) {
    child->mParent = this;
    mChildren.push_back(std::move(child));
}

SceneNode::Ptr SceneNode::detachChild(const SceneNode &node) {
    auto found = std::find_if(mChildren.begin(), mChildren.end(), [&] (Ptr & p) {return p.get() == &node; });
    assert(found != mChildren.end());

    Ptr result = std::move(*found);
    result->mParent = nullptr;
    mChildren.erase(found);
    return result;
}

void SceneNode::update(sf::Time dt) {
    updateCurrent(dt);
    updateChildren(dt);
}

void SceneNode::updateCurrent(sf::Time dt) {
    // To define in inheriting classes
}

void SceneNode::updateChildren(sf::Time dt) {
    FOREACH(Ptr& child, mChildren)
        child->update(dt);
}

void SceneNode::draw(sf::RenderTarget &target, sf::RenderStates states) const {
    states.transform *= getTransform();

    drawCurrent(target, states);
    drawChildren(target, states);

    // Debug :
    //drawBoundingRect(target, states);
}

void SceneNode::drawCurrent(sf::RenderTarget &target, sf::RenderStates states) const {
    // To define in inheriting classes
}

void SceneNode::drawChildren(sf::RenderTarget &target, sf::RenderStates states) const {
    FOREACH(const Ptr& child, mChildren)
        child->draw(target, states);
}

sf::FloatRect SceneNode::getBoundingRect() const
{
    return sf::FloatRect();
}

void SceneNode::drawBoundingRect(sf::RenderTarget& target, sf::RenderStates) const
{
    sf::FloatRect rect = getBoundingRect();

    sf::RectangleShape shape;
    shape.setPosition(sf::Vector2f(rect.left, rect.top));
    shape.setSize(sf::Vector2f(rect.width, rect.height));
    shape.setFillColor(sf::Color::Transparent);
    shape.setOutlineColor(sf::Color::Green);
    shape.setOutlineThickness(1.f);

    target.draw(shape);
}
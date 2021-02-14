//
// Created by 33771 on 13/02/2021.
//

#include "SpriteNode.hpp"

#include <SFML/Graphics/RenderTarget.hpp>

SpriteNode::SpriteNode(const sf::Texture &texture)
: mSprite(texture) {

}

SpriteNode::SpriteNode(const sf::Texture &texture, const sf::IntRect &textureRect)
: mSprite(texture, textureRect) {

}

void SpriteNode::drawCurrent(sf::RenderTarget &target, sf::RenderStates states) const {
    target.draw(mSprite, states);
}
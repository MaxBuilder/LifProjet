//
// Created by 33771 on 13/02/2021.
//

#ifndef LIFPROJET_SPRITENODE_HPP
#define LIFPROJET_SPRITENODE_HPP

#include "SceneNode.hpp"

#include <SFML/Graphics/Sprite.hpp>

class SpriteNode : public SceneNode {
public:
    explicit SpriteNode(const sf::Texture& texture);
    SpriteNode(const sf::Texture& texture, const sf::IntRect& textureRect);

private:
    virtual void drawCurrent(sf::RenderTarget& target, sf::RenderStates states) const;

private:
    sf::Sprite mSprite;

};


#endif //LIFPROJET_SPRITENODE_HPP

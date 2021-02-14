//
// Created by 33771 on 14/02/2021.
//

#ifndef LIFPROJET_TEXTNODE_HPP
#define LIFPROJET_TEXTNODE_HPP

#include "../../Game/Resources/ResourceHolder.hpp"
#include "../../Game/Resources/ResourceIdentifiers.hpp"
#include "SceneNode.hpp"

#include <SFML/Graphics/Text.hpp>
#include <SFML/Graphics/Font.hpp>

class TextNode : public SceneNode {
public:
    explicit TextNode(const FontHolder& fonts, const std::string& text);
    void setString(const std::string& text);

private:
    virtual void drawCurrent(sf::RenderTarget& target, sf::RenderStates states) const;

private:
    sf::Text mText;
};


#endif //LIFPROJET_TEXTNODE_HPP

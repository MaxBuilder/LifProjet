//
// Created by 33771 on 01/02/2021.
//

#include "Utility.hpp"

void centerOrigin(sf::Sprite& sprite)
{
    sf::FloatRect bounds = sprite.getLocalBounds();
    sprite.setOrigin(std::floor(bounds.left + bounds.width / 2.f), std::floor(bounds.top + bounds.height / 2.f));
}

void centerOrigin(sf::Text& text)
{
    sf::FloatRect bounds = text.getLocalBounds();
    text.setOrigin(std::floor(bounds.left + bounds.width / 2.f), std::floor(bounds.top + bounds.height / 2.f));
}

void reverseOrigin(sf::Text& text) {
    sf::FloatRect bounds = text.getLocalBounds();
    text.setOrigin(bounds.left + bounds.width, bounds.top);
}

float angle(const sf::Vector2f& vec1, const sf::Vector2f& vec2 ){
    float rotation;
    if(vec1.x != vec2.x){
        float tan = (vec1.x-vec2.x)/(vec1.y-vec2.y);
        rotation = std::atan(tan)*180/float(M_PI);
        rotation = 90 - rotation;
    }
    else{
        rotation = 90.f;
    }
    if(vec1.y < vec2.y)
        rotation += 180.f;

    return rotation;
}

float norm(const sf::Vector2f& vec) {
    return std::sqrt(vec.x * vec.x + vec.y * vec.y);
}

float distance(const sf::Vector2f& vec1, const sf::Vector2f& vec2) {
    return std::sqrt((vec2.x - vec1.x) * (vec2.x - vec1.x) + (vec2.y - vec1.y) * (vec2.y - vec1.y));
}
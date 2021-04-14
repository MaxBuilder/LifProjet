//
// Created by 33771 on 31/01/2021.
//

#ifndef LIFPROJET_UTILITY_HPP
#define LIFPROJET_UTILITY_HPP

#include <sstream>
#include <cmath>

#include <SFML/Graphics/Text.hpp>
#include <SFML/Graphics/Sprite.hpp>

template <typename T>
std::string toString(const T& value)
{
    std::stringstream stream;
    stream << value;
    return stream.str();
}

void centerOrigin(sf::Sprite& sprite);
void centerOrigin(sf::Text& text);

float toDegree(float radian);
float toRadian(float degree);

float angle(const sf::Vector2f& vec1, const sf::Vector2f& vec2 );
float norm(const sf::Vector2f& vec);
float distance(const sf::Vector2f& vec1, const sf::Vector2f& vec2);
void display(sf::Vector2f vec);

#endif //LIFPROJET_UTILITY_HPP

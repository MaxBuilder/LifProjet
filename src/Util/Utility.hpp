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

#endif //LIFPROJET_UTILITY_HPP

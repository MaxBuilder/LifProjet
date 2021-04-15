//
// Created by 33771 on 31/01/2021.
//

#ifndef LIFPROJET_UTILITY_HPP
#define LIFPROJET_UTILITY_HPP

#include <sstream>
#include <cmath>

#include <SFML/Graphics/Text.hpp>
#include <SFML/Graphics/Sprite.hpp>

/**
 * @brief Converts a value to string
 * @tparam T Type of the variable to convert
 * @param value Value to convert
 * @return Value converted to string
 */
template <typename T>
std::string toString(const T& value)
{
    std::stringstream stream;
    stream << value;
    return stream.str();
}

/**
 * @brief Centers the origin of a sprite
 * @param sprite Sprite to center
 * @author SFML Game Development
 */
void centerOrigin(sf::Sprite& sprite);
/**
 * @brief Conters the origin of a text
 * @param text Text to center
 * @author SFML Game Development
 */
void centerOrigin(sf::Text& text);
/**
 * @brief Reverse to origin of the text on the x axis
 * @param text Text
 */
void reverseOrigin(sf::Text& text);

/**
 * @brief Computes the angle value between two vectors
 * @param vec1 Vector 1
 * @param vec2 Vector 2
 * @return Angle between the 2 vectors
 */
float angle(const sf::Vector2f& vec1, const sf::Vector2f& vec2 );
/**
 * @brief Computes the norm of a vector
 * @param vec Vector
 * @return Norm of the vector
 */
float norm(const sf::Vector2f& vec);
/**
 * @brief Computes the distance between two vectors
 * @param vec1 Vector 1
 * @param vec2 Vector 2
 * @return Distance between the two vectors
 */
float distance(const sf::Vector2f& vec1, const sf::Vector2f& vec2);

#endif //LIFPROJET_UTILITY_HPP

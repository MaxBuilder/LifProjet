//
// Created by 33771 on 31/01/2021.
//

#ifndef LIFPROJET_UTILITY_HPP
#define LIFPROJET_UTILITY_HPP

#include <sstream>

template <typename T>
std::string toString(const T& value)
{
    std::stringstream stream;
    stream << value;
    return stream.str();
}

#endif //LIFPROJET_UTILITY_HPP

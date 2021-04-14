//
// Created by 33771 on 13/04/2021.
//

#ifndef LIFPROJET_DEBUG_HPP
#define LIFPROJET_DEBUG_HPP

#include <string>
#include <iostream>

/**
 * @class Debug
 * @brief Set of functions to display messages in the console
 */
class Debug {
public:
    /**
     * @brief Deleted constructor (static functions only)
     */
    Debug() = delete;

    /**
     * @brief Logs an informationnal message in the console
     * @param message Message to log
     */
    static void Log(const std::string& message) {
        std::cout << "[INFO] " << message << std::endl;
    }

    /**
     * @brief Logs a warning message in the console
     * @param message Message to log
     */
    static void WarningLog(const std::string& message) {
        std::cout << "[WARNING] " << message << std::endl;
    }
};


#endif //LIFPROJET_DEBUG_HPP

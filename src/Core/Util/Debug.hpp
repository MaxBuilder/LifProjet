//
// Created by 33771 on 13/04/2021.
//

#ifndef LIFPROJET_DEBUG_HPP
#define LIFPROJET_DEBUG_HPP

#include <string>
#include <iostream>

class Debug {
public:
    Debug() = delete;

    static void Log(const std::string& message) {
        std::cout << "[INFO] " << message << std::endl;
    }

    static void WarningLog(const std::string& message) {
        std::cout << "[WARNING] " << message << std::endl;
    }
};


#endif //LIFPROJET_DEBUG_HPP

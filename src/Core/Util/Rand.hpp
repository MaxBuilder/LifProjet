//
// Created by 33771 on 31/01/2021.
//

#ifndef LIFPROJET_RAND_HPP
#define LIFPROJET_RAND_HPP

#include<random>
#include<chrono>

namespace {

    /**
     * @class Random
     * @brief Set of function to generate random numbers
     */
    class Random final {
    public:
        /**
         * @brief Deleted constructor (static functions only)
         */
        Random() = delete;

        /***
         * @brief Generate an integer between to limits (inclusive)
         * @param p_min Minimum
         * @param p_max Maximum
         * @return Random integer number
         */
        static int Generate(int p_min, int p_max);

        /***
         * @brief Generate a float between to limits (inclusive)
         * @param p_min Minimum
         * @param p_max Maximum
         * @return Random float number
         */
        static float Generate(float p_min, float p_max);

    private:
        static std::default_random_engine generator;
    };
}

std::default_random_engine Random::generator = std::default_random_engine(std::chrono::steady_clock::now().time_since_epoch().count()); // à modifier

int Random::Generate(int p_min, int p_max) {
    std::uniform_int_distribution<int> distribution(p_min, p_max);
    return distribution(generator);
}

float Random::Generate(float p_min, float p_max) {
    std::uniform_real_distribution<float> distribution(p_min, p_max);
    return distribution(generator);
}

#endif //LIFPROJET_RAND_HPP
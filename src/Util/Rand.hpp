//
// Created by 33771 on 31/01/2021.
//

#ifndef LIFPROJET_RAND_HPP
#define LIFPROJET_RAND_HPP

#include<random>
#include<ctime>

class Random final {
public:
    Random() = delete;

    static int Generate(int p_min, int p_max);
    static int Generate(float p_min, float p_max);
    static bool TryPercentage(float p_percentage);

private:
    static std::default_random_engine generator;
};

std::default_random_engine Random::generator = std::default_random_engine(static_cast<unsigned int>(time(0))); // à modifier

int Random::Generate(int p_min, int p_max) {
    std::uniform_int_distribution<int> distribution(p_min, p_max);
    return distribution(generator);
}

int Random::Generate(float p_min, float p_max) {
    std::uniform_real_distribution<float> distribution(p_min, p_max);
    return distribution(generator);
}

bool Random::TryPercentage(float p_percentage) {
    std::uniform_real_distribution<float> distribution(0.0f, 100.0f);
    return distribution(generator);
}

#endif //LIFPROJET_RAND_HPP

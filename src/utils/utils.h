#pragma once

#include <random>

inline double randf() {
    std::random_device random_device;
    std::mt19937 gen(random_device());

    std::uniform_real_distribution<> distribution(0.0, 1.0);

    return distribution(gen);
}
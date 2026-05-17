#pragma once

#include <random>

#include <windows.h>
#include <string>

inline double randf() {
    std::random_device random_device;
    std::mt19937 gen(random_device());

    std::uniform_real_distribution<> distribution(0.0, 1.0);

    return distribution(gen);
}

// Generated with AI
inline std::string getExePath() {
    char buffer[MAX_PATH];
    GetModuleFileNameA(NULL, buffer, MAX_PATH);
    std::string fullPath(buffer);

    size_t pos = fullPath.find_last_of("\\/");
    return fullPath.substr(0, pos);
}
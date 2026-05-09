#pragma once

#include <random>
#include <Eigen/Dense>

#include "../logger/Logger.h"

using Eigen::MatrixXd;

inline double randf() {
    std::random_device random_device;
    std::mt19937 gen(random_device());

    std::uniform_real_distribution<> distribution(0.0, 1.0);

    return distribution(gen);
}

inline void print_image_matrix(const MatrixXd& img) {
    const std::string chars = " .:-=+*#%@";

    std::string line = "+-";
    for (int x = 0; x < img.cols(); x++) {
        line += "--";
    }
    Logger::log(line + "-+");


    double min_val = img.minCoeff();
    double max_val = img.maxCoeff();
    double range   = max_val - min_val;

    for (int y = 0; y < img.rows(); y++) {
        line = "| ";
        for (int x = 0; x < img.cols(); x++) {
            double normalized = (range > 0) ? (img(y, x) - min_val) / range : 0.0;
            int index = static_cast<int>(normalized * (chars.size() - 1));
            line += chars[index];
            line += chars[index];
        }
        Logger::log(line +" |");
    }


    line = "+-";
    for (int x = 0; x < img.cols(); x++) {
        line += "--";
    }
    Logger::log(line + "-+");
}
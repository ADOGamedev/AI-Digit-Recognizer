#pragma once

#include <random>
#include <vector>

#include <Eigen/Dense>

using Eigen::VectorXd, Eigen::MatrixXd;
using Tensor = std::vector<MatrixXd>;

const int KERNEL_SIZE = 3;

struct Kernel {
    Kernel() = default;
    Kernel(const int c, const int s) {
        size = s;
        channels = c;

        values.resize(channels);

        for (int i = 0; i < channels; i++) {
            values[i] = MatrixXd::Zero(s, s);
        }

        perform_he_initialization();
    }

    void perform_he_initialization() {
        int fan_in = channels * size * size;
        double std_deviation = std::sqrt(2.0 / fan_in);

        std::random_device rd;
        std::mt19937 gen(rd());
        std::normal_distribution<double> dist(0.0, std_deviation);

        for (MatrixXd& matrix : values) {
            for (int y = 0; y < size; y++) {
                for (int x = 0; x < size; x++) {
                    matrix(y, x) = dist(gen);
                }
            }
        }

        bias = 0.0;
    }

    int size;
    int channels;

    Tensor values;
    double bias;
};
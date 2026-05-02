#pragma once

#include "../logger/Logger.h"
#include "../neural_network/NeuralNetwork.h"

class Trainer {

public:
    Trainer() = default;

    template <size_t N>
    double cost(NeuralNetwork& network, const std::array<double, N> expected) {
        if (network.get_output_layer().neurons_count != N) {
            Logger::logerr("ERROR: Trying to find the cost comparing vectors of different sizes");
            return 0;
        }
        Eigen::Map<const VectorXd> expected_vec(expected.data(), expected.size());
        return (network.get_output_layer().values - expected_vec).squaredNorm();
    }

};
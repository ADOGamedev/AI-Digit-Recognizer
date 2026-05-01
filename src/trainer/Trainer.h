#pragma once

#include "../logger/Logger.h"
#include "../neural_network/NeuralNetwork.h"

class Trainer {

public:
    template <size_t N>
    double cost(const NeuralNetwork& network, const std::array<double, N> expected) {
        if (network.get_output_layer().size() != N) {
            Logger::logerr("ERROR: Trying to find the cost comparing vectors of different sizes")
            return 0
        }
        return (network.get_output_layer().values - expected).array().squaredNorm();
    }
    
};
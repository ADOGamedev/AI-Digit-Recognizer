#pragma once

#include <array>

#include <Eigen/Dense>

#include "../logger/Logger.h"

using Eigen::VectorXd, Eigen::MatrixXd;

struct Layer {
    Layer() : neurons_count(0), values(VectorXd::Zero(0)), biases(VectorXd::Zero(0)) {}
    Layer(int n) :
        neurons_count(n),
        values(VectorXd::Zero(n)),
        biases(VectorXd::Zero(n)) {}
    
    int neurons_count;
    VectorXd values;
    VectorXd biases;
};

struct WeightMatrix {
    WeightMatrix() : matrix(MatrixXd::Zero(0, 0)) {}
    WeightMatrix(int outputs, int inputs) :
        matrix(MatrixXd::Zero(outputs, inputs)) {}

    MatrixXd matrix;
};


class NeuralNetwork {

public:
    template <size_t N, size_t W>
    NeuralNetwork(std::array<Layer, N>& layers, std::array<WeightMatrix, W>& weights) {
        const int hidden_layers_count = N - 2;
        const int weight_matrices_count = std::min(W, N - 1);

        hidden_layers.resize(hidden_layers_count);
        weight_matrices.resize(weight_matrices_count);

        input_layer = layers[0];
        output_layer = layers[N - 1];

        for (int i = 1; i < hidden_layers_count; i++) {
            hidden_layers[i - 1] = layers[i];
        }

        for (int i = 0; i < weight_matrices_count; i++) {
            weight_matrices[i] = weights[i];
        }
    }

private:
    Layer input_layer;
    std::vector<Layer> hidden_layers;
    Layer output_layer;

    std::vector<WeightMatrix> weight_matrices;

};
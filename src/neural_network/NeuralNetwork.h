#pragma once

#include <array>
#include <fstream>

#include <Eigen/Dense>
#include <json.hpp>

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
    NeuralNetwork() = default;
    
    template <size_t N, size_t W>
    NeuralNetwork(std::array<Layer, N>& layers_arr, std::array<WeightMatrix, W>& weights) {
        const int weight_matrices_count = std::min(W, N - 1);

        layers.resize(N);
        weight_matrices.resize(weight_matrices_count);

        layers_count = N;

        for (int i = 0; i < N; i++) {
            layers[i] = layers_arr[i];
        }

        for (int i = 0; i < weight_matrices_count; i++) {
            weight_matrices[i] = weights[i];
        }
    }

    void load_from_json(const std::string& path);
    void compute_output();
    
    void set_input_layer(const VectorXd& values);
    Layer get_output_layer();

    Layer get_layer(int index);
    WeightMatrix get_weight_matrix(int index);
    

private:
    VectorXd fast_sigmoid(const VectorXd& v);

    int layers_count;
    std::vector<Layer> layers;
    
    std::vector<WeightMatrix> weight_matrices;
    
};
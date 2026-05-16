#pragma once

#include <array>
#include <fstream>

#include <Eigen/Dense>
#include <json.hpp>

#include "../../utils/utils.h"
#include "../../logger/Logger.h"

using Eigen::VectorXd, Eigen::MatrixXd;

struct Layer {
    Layer() : neurons_count(0), values(VectorXd::Zero(0)), biases(VectorXd::Zero(0)) {}
    Layer(int n) :
        neurons_count(n),
        values(VectorXd::Zero(n)),
        biases(VectorXd::Zero(n)),
        z_values(VectorXd::Zero(n)) {}

    std::vector<double> biases_to_list() const {
        return std::vector<double>(biases.data(), biases.data() + neurons_count);
    }

    int neurons_count;
    VectorXd values;
    VectorXd biases;
    VectorXd z_values;
};

struct WeightMatrix {
    WeightMatrix() : matrix(MatrixXd::Zero(0, 0)) {}
    WeightMatrix(int outputs, int inputs) :
        outputs(outputs), inputs(inputs), matrix(MatrixXd::Zero(outputs, inputs)) {}

    std::vector<std::vector<double>> to_list() const {
        std::vector<std::vector<double>> list;
        list.resize(outputs);
        for (int j = 0; j < outputs; j++) {
            list[j].resize(inputs);
            for (int i = 0; i < inputs; i++) {
                list[j][i] = matrix(j, i);
            }
        }

        return list;
    }

    int outputs;
    int inputs;
    MatrixXd matrix;
};


class FCNeuralNetwork {

public:
    FCNeuralNetwork() = default;

    template <size_t N>
    FCNeuralNetwork(std::array<int, N> neurons_per_layer) {
        layers_count = N;

        layers.resize(N);
        weight_matrices.resize(N - 1);

        for (int i = 0; i < N; i++) {
            layers[i] = Layer(neurons_per_layer[i]);
        }

        for (int i = 0; i < N - 1; i++) {
            weight_matrices[i] = WeightMatrix(neurons_per_layer[i + 1], neurons_per_layer[i]);
        }

        randomize_weights_xavier();
    }
    
    template <size_t N, size_t W>
    FCNeuralNetwork(std::array<Layer, N>& layers_arr, std::array<WeightMatrix, W>& weights) {
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

    void randomize_weights_xavier();

    void load_from_json(const std::string& path);
    void save_to_json(const std::string& path);
    void compute_output();
    
    void set_input_layer(const VectorXd& values);
    Layer get_output_layer();

    Layer get_layer(int index);
    WeightMatrix get_weight_matrix(int index);

    void change_biases(const int layer_index, const VectorXd& amount);
    void change_weights(const int weight_index, const MatrixXd& amount);
    
    VectorXd sigmoid(const VectorXd& v);
    
    int layers_count;

private:
    std::vector<Layer> layers;
    
    std::vector<WeightMatrix> weight_matrices;
    
};
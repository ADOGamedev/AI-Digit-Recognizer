#pragma once

#include "../logger/Logger.h"
#include "../neural_network/fully_connected/FCNeuralNetwork.h"

#include "../mnist_dataset/MNISTDataset.h"


struct CostGradient {
    CostGradient() = default;
    
    std::vector<VectorXd> biases;
    std::vector<MatrixXd> weights;

    CostGradient& operator+=(const CostGradient& other) {
        if (biases.empty()) {
            *this = other;
            return *this;
        }
        
        for (int i = 0; i < biases.size(); i++)
            biases[i] += other.biases[i];
        for (int i = 0; i < weights.size(); i++)
            weights[i] += other.weights[i];

        return *this;
    }
};


class Trainer {

public:
    Trainer() = default;
    Trainer(FCNeuralNetwork* neural_net, double learning_rate) :
        network(neural_net), learning_rate(learning_rate) {}

    
    void train(int epochs, bool test_after_epoch=false);
    void update_net_for_a_single_batch(std::vector<MNISTImageLabel>& batch);
    CostGradient backprop(const MNISTImageLabel& example);

    VectorXd cost_derivative(const VectorXd& expected);
    VectorXd fast_sigmoid_derivative(const VectorXd& vec);

    double test_network();

    void set_learning_rate(const double& v) { learning_rate = v; }
    
private:
    FCNeuralNetwork* network;
    double learning_rate;
};
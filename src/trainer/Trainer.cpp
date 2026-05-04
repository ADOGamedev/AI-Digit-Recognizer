#include "Trainer.h"

void Trainer::train(int epochs, bool test_after_epoch) {
    for (int i = 0; i < epochs; i++) {
        for (std::vector<MNISTImageLabel>& batch : MNISTDataset::batches) {
            update_net_for_a_single_batch(batch);
        }

        if (test_after_epoch) {
            Logger::log("-----------------------------");
            Logger::log("Epoch " + std::to_string(i));
            test_network();
        }
    }
}

void Trainer::update_net_for_a_single_batch(std::vector<MNISTImageLabel>& batch) {
    CostGradient accumulated;

    for (MNISTImageLabel& example : batch) {
        network->set_input_layer(example.image_values);
        network->compute_output();
        
        accumulated += backprop(example);
    }

    for (int i = 0; i < accumulated.biases.size(); i++) {
        /*std::ostringstream oss;
        oss << (-learning_rate / batch.size() * accumulated.biases[i]);
        Logger::log("Changing bias " + std::to_string(i + 1) + " by \n" + oss.str());*/
        network->change_biases(i + 1, -learning_rate * accumulated.biases[i]);
    }
    for (int i = 0; i < accumulated.weights.size(); i++) {
        network->change_weights(i, -learning_rate * accumulated.weights[i]);
    }
    
}


CostGradient Trainer::backprop(const MNISTImageLabel& example) {
    CostGradient cost_gradient;
    
    VectorXd error = cost_derivative(example.label).cwiseProduct(fast_sigmoid_derivative(network->get_output_layer().z_values));

    cost_gradient.biases.resize(network->layers_count - 1);
    cost_gradient.weights.resize(network->layers_count - 1);
    
    cost_gradient.biases[network->layers_count - 2] = error;
    cost_gradient.weights[network->layers_count - 2] = error * network->get_layer(network->layers_count - 2).values.transpose();

    for (int i = network->layers_count - 3; i >= 0; i--) {
        error = 
            (network->get_weight_matrix(i + 1).matrix.transpose() * error)
            .cwiseProduct(
            fast_sigmoid_derivative(
            network->get_layer(i + 1).z_values)
        );

        cost_gradient.biases[i] = error;
        cost_gradient.weights[i] = error * network->get_layer(i).values.transpose();
    }

    return cost_gradient;
}


VectorXd Trainer::cost_derivative(const VectorXd& expected) {
    return 2 * (network->get_output_layer().values - expected);
}


VectorXd Trainer::fast_sigmoid_derivative(const VectorXd& vec) {
    auto s = network->sigmoid(vec).array();
    return (s * (1.0 - s)).matrix();
}


double Trainer::test_network() {
    int correct_examples = 0;
    int total_examples = MNISTDataset::test_examples.size();
    Logger::log("Testing with " + std::to_string(total_examples) + " examples");

    for (MNISTImageLabel& test_example : MNISTDataset::test_examples) {
        network->set_input_layer(test_example.image_values);
        network->compute_output();

        int guessed_digit;
        network->get_output_layer().values.maxCoeff(&guessed_digit);

        int correct_digit;
        test_example.label.maxCoeff(&correct_digit);

        if (correct_digit == guessed_digit) {
            correct_examples++;
        }
    }

    double accuracy = correct_examples / (double)total_examples;

    Logger::log("Results: " +
        std::to_string(correct_examples) + " / " + std::to_string(total_examples) + "\n" +
        "Accuracy: "  + std::to_string(accuracy)
    );

    return accuracy;
}
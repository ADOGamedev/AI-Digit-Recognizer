#include "NeuralNetwork.h"


void NeuralNetwork::load_from_json(const std::string& path) {
    std::ifstream json_file(path);
    nlohmann::json data = nlohmann::json::parse(json_file);

    layers_count = data["biases"].size() + 1;
    if (layers_count <= 0) {
        return;
    }

    int weights_count = std::min(layers_count - 1, (int)data["weights"].size());

    layers.resize(layers_count);
    weight_matrices.resize(weights_count);
    layers[0] = Layer(data["inputs"]);

    for (int i = 1; i < layers_count; i++) {
        auto biases = data["biases"][i - 1];
        int neurons_count = biases.size();

        Layer new_layer = Layer(neurons_count);

        for (int j = 0; j < neurons_count; j++) {
            new_layer.biases(j) = biases[j]; 
        }

        layers[i] = new_layer;
    }

    for (int i = 0; i < weights_count; i++) {
        auto weights_arr = data["weights"][i];
        int outputs = weights_arr.size();
        
        if (outputs <= 0) continue;

        int inputs = weights_arr[0].size();

        WeightMatrix new_weight_matrix(outputs, inputs);

        for (int j = 0; j < outputs; j++) {
            for (int k = 0; k < inputs; k++) {
                new_weight_matrix.matrix(j, k) = weights_arr[j][k];
            }
        }

        weight_matrices[i] = new_weight_matrix;
    }
}


void NeuralNetwork::compute_output() {
    for (int i = 0; i < layers_count - 1; i++) {
        auto& z = weight_matrices[i].matrix * layers[i].values + layers[i + 1].biases;
        layers[i + 1].values = fast_sigmoid(z);
    }
}

void NeuralNetwork::set_input_layer(const VectorXd& values) {
    if (layers[0].values.size() != values.size()) {
        Logger::logerr("ERROR: trying to set inputs with " + std::to_string(values.size()) + " values");
        return;
    }
    layers[0].values = values;
}


Layer NeuralNetwork::get_output_layer() {
    return get_layer(layers_count - 1);
}


Layer NeuralNetwork::get_layer(int index) {
    if (index < layers_count) {
        return layers[index];
    }
    else {
        Logger::logerr("ERROR: trying to get a layer at index " + std::to_string(index));
    }
}

WeightMatrix NeuralNetwork::get_weight_matrix(int index) {
    if (index < weight_matrices.size()) {
        return weight_matrices[index];
    }
    else {
        Logger::logerr("ERROR: trying to get a weight matrix at index " + std::to_string(index));
    }
}

VectorXd NeuralNetwork::fast_sigmoid(const VectorXd& v) {
    VectorXd result = v;
    auto a = result.array();
    result = 0.5 * (a / (1 + a.abs()) + 1);
    return result;
}
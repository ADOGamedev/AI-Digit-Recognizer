#include "FCNeuralNetwork.h"


void FCNeuralNetwork::randomize_weights_xavier() {
    for (int i = 0; i < weight_matrices.size(); i++) {
        int n_in = layers[i].neurons_count;
        int n_out = layers[i + 1].neurons_count;

        double limit = sqrt(6.0 / (n_in + n_out));

        weight_matrices[i].matrix = MatrixXd::Random(n_out, n_in) * limit;
    }
}


void FCNeuralNetwork::load_from_json(const std::string& path) {
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

void FCNeuralNetwork::save_to_json(const std::string& path) {
    nlohmann::json data;
    data["inputs"] = layers[0].neurons_count;

    data["biases"] = nlohmann::json::array();
    for (int i = 1; i < layers_count; i++) {
        data["biases"].push_back(layers[i].biases_to_list());
    }

    data["weights"] = nlohmann::json::array();
    for (const WeightMatrix& weights : weight_matrices) {
        data["weights"].push_back(weights.to_list());
    }

    std::ofstream file(path);
    if (!file.is_open()) {
        Logger::logerr("ERROR: JSON could not be opened");
        return;
    }
    file << data.dump(4);
    file.close();
}

void FCNeuralNetwork::compute_output() {
    for (int i = 0; i < layers_count - 1; i++) {
        VectorXd z = weight_matrices[i].matrix * layers[i].values + layers[i + 1].biases;
        layers[i + 1].z_values = z;
        layers[i + 1].values = sigmoid(z);
    }
}

void FCNeuralNetwork::set_input_layer(const VectorXd& values) {
    if (layers[0].neurons_count != values.size()) {
        Logger::logerr("ERROR: trying to set inputs with " + std::to_string(values.size()) + " values");
        return;
    }
    layers[0].values = values;
}


Layer FCNeuralNetwork::get_output_layer() {
    return get_layer(layers_count - 1);
}


Layer FCNeuralNetwork::get_layer(int index) {
    if (index < layers_count) {
        return layers[index];
    }
    else {
        Logger::logerr("ERROR: trying to get a layer at index " + std::to_string(index));
    }
}

WeightMatrix FCNeuralNetwork::get_weight_matrix(int index) {
    if (index < weight_matrices.size()) {
        return weight_matrices[index];
    }
    else {
        Logger::logerr("ERROR: trying to get a weight matrix at index " + std::to_string(index));
    }
}

void FCNeuralNetwork::change_biases(const int layer_index, const VectorXd& amount) {
    if (layer_index >= layers.size()) {
        Logger::log("ERROR: trying to change the bias of an out-out-bounds layer");
        return;
    }
    layers[layer_index].biases += amount;
}


void FCNeuralNetwork::change_weights(const int weight_index, const MatrixXd& amount) {
    if (weight_index >= weight_matrices.size()) {
        Logger::log("ERROR: trying to change the weights of an out-out-bounds weight matrix");
        return;
    }
    weight_matrices[weight_index].matrix += amount;
}


VectorXd FCNeuralNetwork::sigmoid(const VectorXd& v) {
    return (1.0 / (1.0 + (-v.array()).exp())).matrix();
}
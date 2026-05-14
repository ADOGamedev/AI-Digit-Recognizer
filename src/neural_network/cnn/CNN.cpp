#include "CNN.h"

CNN::CNN(const std::vector<int>& kernels_count_per_layer) {
    layers_count = kernels_count_per_layer.size();
    layers.reserve(layers_count);

    for (int i = 0; i < layers_count; i++) {
        int channels = i <= 0 ? 1 : kernels_count_per_layer[i - 1];
        layers.emplace_back(channels, kernels_count_per_layer[i]);
    }
}

void CNN::load_from_json(const std::string& path) {
    std::ifstream json_file(path);
    nlohmann::json data = nlohmann::json::parse(json_file);

    layers_count = data["biases"].size();
    layers.resize(layers_count);

    for (int i = 0; i < layers_count; i++) {
        std::vector<Kernel> kernels;
        kernels.resize(data["kernels"][i].size());

        for (int j = 0; j < data["kernels"][i].size(); j++) {
            kernels[j] = array_3D_to_kernel(data["kernels"][i][j]);
        }

        layers[i].set_kernels(kernels);
    }

    for (int i = 0; i < layers_count; i++) {
        for (int j = 0; j < data["biases"][i].size(); j++) {
            double bias = data["biases"][i][j];
            layers[i].set_kernel_bias(j, bias);
        }
    }

    json_file.close();
}


void CNN::save_from_json(const std::string& path) {
    nlohmann::json data;

    data["biases"] = nlohmann::json::array();
    for (int i = 0; i < layers_count; i++) {
        data["biases"].push_back(layers[i].biases_to_list());
    }

    data["kernels"] = nlohmann::json::array();
    for (int i = 0; i < layers_count; i++) {
        data["kernels"].push_back(layers[i].kernels_to_list());
    }


    std::ofstream file(path);
    if (!file.is_open()) {
        Logger::logerr("ERROR: JSON could not be opened");
        return;
    }
    file << data.dump(4);
    file.close();
}


Kernel CNN::array_3D_to_kernel(const nlohmann::json& array) {
    int channels = array.size();
    int size = channels > 0 ? array[0].size() : 0;
    Kernel kernel(channels, size);

    for (int c = 0; c < channels; c++) {
        for (int y = 0; y < size; y++) {
            for (int x = 0; x < size; x++) {
                kernel.values[c](y, x) = array[c][y][x];
            }
        }
    }

    return kernel;
}


void CNN::set_input(const Tensor& input) {
    layers[0].set_image_tensor(input);
}

void CNN::do_forward_pass() {
    for (int i = 0; i < layers_count; i++) {
        Tensor processed_img = layers[i].compute_output();

        if (i < layers_count - 1) {
            layers[i + 1].set_image_tensor(processed_img);
        }
        else {
            output = processed_img;
        }
    }
}

Tensor CNN::get_output_tensor() {
    return output;
}

Tensor CNN::get_img_tensor(const int layer) {
    return layers[layer].get_image_tensor();
}

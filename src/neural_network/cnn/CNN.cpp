#include "CNN.h"

CNN::CNN(const std::vector<int>& kernels_count_per_layer) {
    layers_count = kernels_count_per_layer.size();
    layers.resize(layers_count);

    for (int i = 0; i < layers_count; i++) {
        int channels = i <= 0 ? 1 : kernels_count_per_layer[i - 1];
        layers.emplace_back(channels, kernels_count_per_layer[i]);
    }
}

void CNN::set_input(const Tensor& input) {
    layers[0].set_image_tensor(input);
}
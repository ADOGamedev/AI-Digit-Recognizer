#pragma once

#include <random>
#include <vector>

#include <Eigen/Dense>

#include "ConvolutionalLayer.h"


class CNN  {

public:
    CNN() = default;
    CNN(const std::vector<int>& kernels_count_per_layer);

    void set_input(const Tensor& input);

private:
    int layers_count;
    std::vector<ConvolutionalLayer> layers;
};
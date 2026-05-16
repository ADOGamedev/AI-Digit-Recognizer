#pragma once

#include <iostream>
#include <random>
#include <vector>

#include <array>
#include <fstream>

#include <Eigen/Dense>
#include <json.hpp>

#include "ConvolutionalLayer.h"


class CNN  {

public:
    CNN() = default;
    CNN(const std::vector<int>& kernels_count_per_layer);

    void load_from_json(const std::string& path);
    void save_from_json(const std::string& path);

    Kernel array_3D_to_kernel(const nlohmann::json& array);


    void set_input(const Tensor& input);

    void do_forward_pass();

    Tensor get_output_tensor();
    Tensor get_img_tensor(const int layer);

    int layers_count;
private:

    std::vector<ConvolutionalLayer> layers;
    Tensor output;
};
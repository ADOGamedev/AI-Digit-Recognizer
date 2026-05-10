#pragma once

#include "Kernel.h"

using Eigen::VectorXd, Eigen::MatrixXd;
using Tensor = std::vector<MatrixXd>;

class ConvolutionalLayer {

public:
    ConvolutionalLayer() = default;
    ConvolutionalLayer(const int channels, const int kernels_count);

    Tensor compute_output();

    Tensor do_convolution(const Tensor& img_tensor);
    MatrixXd get_filtered_image(const Tensor& img_tensor, const Kernel& kernel);

    Tensor do_max_pooling(const Tensor& img_tensor);
    MatrixXd max_pool_for_single_img(const MatrixXd& img);


    void set_image_tensor(const Tensor& img_tensor);
    Tensor get_image_tensor();

    void set_kernels(std::vector<Kernel>& k);
    void set_kernel_bias(const int index, const double bias);

    double ReLU(const double z) { return std::max(0.0, z); }
    
    Tensor image_tensor;
    std::vector<Kernel> kernels;
    Tensor output_tensor;
};
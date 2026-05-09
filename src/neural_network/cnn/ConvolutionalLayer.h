#pragma once

#include "Kernel.h"

using Eigen::VectorXd, Eigen::MatrixXd;
using Tensor = std::vector<MatrixXd>;

class ConvolutionalLayer {

public:
    ConvolutionalLayer() = default;
    ConvolutionalLayer(const int channels, const int kernels_count);

    Tensor do_convolution();
    MatrixXd get_filtered_image(const Kernel& kernel);

    Tensor do_max_pooling();
    MatrixXd max_pool_for_single_img(const MatrixXd& img);


    void set_image_tensor(const Tensor& img_tensor);

    double ReLU(const double z) { return std::max(0.0, z); }
    

    Tensor image_tensor;
    std::vector<Kernel> kernels;
private:

};
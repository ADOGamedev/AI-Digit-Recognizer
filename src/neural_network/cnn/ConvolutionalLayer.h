#pragma once

#include "Kernel.h"

#include "../../utils/utils.h"

using Eigen::VectorXd, Eigen::MatrixXd;
using Tensor = std::vector<MatrixXd>;

using std_vector_3D = std::vector<std::vector<std::vector<double>>>;
using std_vector_4D = std::vector<std_vector_3D>;

class ConvolutionalLayer {

public:
    ConvolutionalLayer() = default;
    ConvolutionalLayer(const int channels, const int kernels_count);

    Tensor compute_output();

    Tensor do_convolution(const Tensor& img_tensor);
    MatrixXd get_filtered_image(const Tensor& img_tensor, const int kernel_index);

    Tensor do_max_pooling(const Tensor& img_tensor);
    MatrixXd max_pool_for_single_img(const MatrixXd& img, const int img_index);


    void set_image_tensor(const Tensor& img_tensor);
    Tensor get_image_tensor();

    void set_kernels(std::vector<Kernel>& k);
    void set_kernel_bias(const int index, const double bias);

    std::vector<double> biases_to_list();
    std_vector_4D kernels_to_list();

    double ReLU(const double z) { return std::max(0.0, z); }
    
    Tensor image_tensor;
    std::vector<Kernel> kernels;
    Tensor z_values_tensor;

    Tensor max_pool_rows;
    Tensor max_pool_cols;

    Tensor output_tensor;
};
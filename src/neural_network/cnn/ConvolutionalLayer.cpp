#include "ConvolutionalLayer.h"

ConvolutionalLayer::ConvolutionalLayer(const int channels, const int kernels_count) {
    kernels.reserve(kernels_count);

    for (int i = 0; i < kernels_count; i++) {
        kernels.emplace_back(channels, KERNEL_SIZE);
    }
}

Tensor ConvolutionalLayer::compute_output() {
    output_tensor = do_max_pooling(do_convolution(image_tensor));
    return output_tensor;
}

Tensor ConvolutionalLayer::do_convolution(const Tensor& img_tensor) {
    Tensor result;
    result.resize(kernels.size());
    z_values_tensor.resize(kernels.size());

    for (int i = 0; i < kernels.size(); i++) {
        result[i] = get_filtered_image(img_tensor, i);
    }

    return result;
} 

MatrixXd ConvolutionalLayer::get_filtered_image(const Tensor& img_tensor, const int kernel_index) {
    Kernel& kernel = kernels[kernel_index];

    MatrixXd filtered_image = MatrixXd::Zero(img_tensor[0].rows() - KERNEL_SIZE + 1, img_tensor[0].cols() - KERNEL_SIZE + 1);
    z_values_tensor[kernel_index] = MatrixXd::Zero(img_tensor[0].rows() - KERNEL_SIZE + 1, img_tensor[0].cols() - KERNEL_SIZE + 1);
    
    for (int y = 0; y < img_tensor[0].rows() - KERNEL_SIZE + 1; y++) {
        for (int x = 0; x < img_tensor[0].cols() - KERNEL_SIZE + 1; x++) {
            double z = 0.0;

            for (int channel = 0; channel < img_tensor.size(); channel++) {
                MatrixXd region = img_tensor[channel].block(y, x, KERNEL_SIZE, KERNEL_SIZE);
                z += kernel.values[channel].cwiseProduct(region).sum();
            }
            z += kernel.bias;

            z_values_tensor[kernel_index](y, x) = z;
            filtered_image(y, x) += ReLU(z);
        }
    }

    return filtered_image;
}


Tensor ConvolutionalLayer::do_max_pooling(const Tensor& img_tensor) {
    Tensor result = Tensor();
    result.resize(img_tensor.size());

    max_pool_rows.resize(kernels.size());
    max_pool_cols.resize(kernels.size());

    for (int i = 0; i < img_tensor.size(); i++) {
        int out_rows = img_tensor[i].rows() / 2;
        int out_cols = img_tensor[i].cols() / 2;
        max_pool_rows[i] = MatrixXd::Zero(out_rows, out_cols);
        max_pool_cols[i] = MatrixXd::Zero(out_rows, out_cols);

        result[i] = max_pool_for_single_img(img_tensor[i], i);
    }

    return result;
}

MatrixXd ConvolutionalLayer::max_pool_for_single_img(const MatrixXd& img, const int img_index) {
    MatrixXd result(img.rows() / 2, img.cols() / 2);

    int max_y_val = img.rows() - (img.rows() % 2);
    int max_x_val = img.cols() - (img.cols() % 2);

    for (int y = 0; y < max_y_val; y += 2) {
        for (int x = 0; x < max_x_val; x += 2) {
            MatrixXd region = img.block(y, x, 2, 2);

            int row;
            int col;

            result(y / 2, x / 2) = region.maxCoeff(&row, &col);

            max_pool_rows[img_index](y / 2, x / 2) = y + row; 
            max_pool_cols[img_index](y / 2, x / 2) = x + col; 
        }
    }

    return result;
}


void ConvolutionalLayer::set_image_tensor(const Tensor& img_tensor) {
    image_tensor = img_tensor;
}

Tensor ConvolutionalLayer::get_image_tensor() {
    return image_tensor;
}


void ConvolutionalLayer::set_kernels(std::vector<Kernel>& k) {
    kernels = k;
}

void ConvolutionalLayer::set_kernel_bias(const int index, const double bias) {
    kernels[index].bias = bias;
}

std::vector<double> ConvolutionalLayer::biases_to_list() {
    std::vector<double> result = {};
    for (Kernel& kernel :  kernels) {
        result.push_back(kernel.bias);
    }

    return result;
}

std_vector_4D ConvolutionalLayer::kernels_to_list() {
    std_vector_4D result = {};
    
    for (Kernel& kernel : kernels) {
        std_vector_3D kernel_vector = {};
        for (int c = 0; c < kernel.channels; c++) {
            kernel_vector.push_back(eigen_matrix_to_vector(kernel.values[c]));
        }
        result.push_back(kernel_vector);
    }

    return result;
}


#include "ConvolutionalLayer.h"

ConvolutionalLayer::ConvolutionalLayer(const int channels, const int kernels_count) {
    kernels.reserve(kernels_count);

    for (int i = 0; i < kernels_count; i++) {
        kernels.emplace_back(channels, KERNEL_SIZE);
    }
}

Tensor ConvolutionalLayer::do_convolution() {
    Tensor result;
    result.resize(kernels.size());

    for (int i = 0; i < kernels.size(); i++) {
        Kernel kernel = kernels[i];
        result[i] = get_filtered_image(kernel);
    }

    return result;
} 

MatrixXd ConvolutionalLayer::get_filtered_image(const Kernel& kernel) {
    MatrixXd filtered_image = MatrixXd::Zero(image_tensor[0].rows() - KERNEL_SIZE + 1, image_tensor[0].cols() - KERNEL_SIZE + 1);
    
    
    for (int y = 0; y < image_tensor[0].rows() - KERNEL_SIZE + 1; y++) {
        for (int x = 0; x < image_tensor[0].cols() - KERNEL_SIZE + 1; x++) {
            double z = 0.0;

            for (int channel = 0; channel < image_tensor.size(); channel++) {
                MatrixXd region = image_tensor[channel].block(y, x, KERNEL_SIZE, KERNEL_SIZE);
                z += kernel.values[channel].cwiseProduct(region).sum();
            }

            filtered_image(y, x) += ReLU(z);
        }
    }

    return filtered_image;
}


Tensor ConvolutionalLayer::do_max_pooling() {
    Tensor result = Tensor();
    result.resize(image_tensor.size());

    for (int i = 0; i < image_tensor.size(); i++) {
        result[i] = max_pool_for_single_img(image_tensor[i]);
    }

    return result;
}

MatrixXd ConvolutionalLayer::max_pool_for_single_img(const MatrixXd& img) {
    MatrixXd result(img.rows() / 2, img.cols() / 2);

    int max_y_val = img.rows() - (img.rows() % 2);
    int max_x_val = img.cols() - (img.cols() % 2);

    for (int y = 0; y < max_y_val; y += 2) {
        for (int x = 0; x < max_x_val; x += 2) {
            MatrixXd region = img.block(y, x, 2, 2);
            result(y / 2, x / 2) = region.maxCoeff();
        }
    }

    return result;
}


void ConvolutionalLayer::set_image_tensor(const Tensor& img_tensor) {
    image_tensor = img_tensor;
}
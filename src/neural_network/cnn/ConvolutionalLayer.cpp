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

    for (int i = 0; i < kernels.size(); i++) {
        Kernel kernel = kernels[i];
        result[i] = get_filtered_image(img_tensor, kernel);
    }

    return result;
} 

MatrixXd ConvolutionalLayer::get_filtered_image(const Tensor& img_tensor, const Kernel& kernel) {
    MatrixXd filtered_image = MatrixXd::Zero(img_tensor[0].rows() - KERNEL_SIZE + 1, img_tensor[0].cols() - KERNEL_SIZE + 1);
    
    for (int y = 0; y < img_tensor[0].rows() - KERNEL_SIZE + 1; y++) {
        for (int x = 0; x < img_tensor[0].cols() - KERNEL_SIZE + 1; x++) {
            double z = 0.0;

            for (int channel = 0; channel < img_tensor.size(); channel++) {
                MatrixXd region = img_tensor[channel].block(y, x, KERNEL_SIZE, KERNEL_SIZE);
                z += kernel.values[channel].cwiseProduct(region).sum();
            }
            //z += bias;
            filtered_image(y, x) += std::abs(z);
        }
    }

    return filtered_image;
}


Tensor ConvolutionalLayer::do_max_pooling(const Tensor& img_tensor) {
    Tensor result = Tensor();
    result.resize(img_tensor.size());

    for (int i = 0; i < img_tensor.size(); i++) {
        result[i] = max_pool_for_single_img(img_tensor[i]);
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

Tensor ConvolutionalLayer::get_image_tensor() {
    return image_tensor;
}


void ConvolutionalLayer::set_kernels(std::vector<Kernel>& k) {
    kernels = k;
}

void ConvolutionalLayer::set_kernel_bias(const int index, const double bias) {
    kernels[index].bias = bias;
}

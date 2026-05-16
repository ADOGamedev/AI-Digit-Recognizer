#pragma once

#include <Eigen/Dense>
#include "mnist_loader.h"

#include "../logger/Logger.h"

using Eigen::VectorXd;

namespace MNISTDataset {
    constexpr int MNIST_IMAGE_SIZE = 28;
    constexpr int IMAGES_PER_BATCH = 20;
    
    inline std::string PATH_TO_TRAINING_LABELS = "G:\\My Projects\\AI Digit Recognizer\\src\\mnist_dataset\\train-labels.idx1-ubyte";
    inline std::string PATH_TO_TRAINING_IMAGES = "G:\\My Projects\\AI Digit Recognizer\\src\\mnist_dataset\\train-images.idx3-ubyte";
    
    inline std::string PATH_TO_TESTING_LABELS = "G:\\My Projects\\AI Digit Recognizer\\src\\mnist_dataset\\t10k-labels.idx1-ubyte";
    inline std::string PATH_TO_TESTING_IMAGES = "G:\\My Projects\\AI Digit Recognizer\\src\\mnist_dataset\\t10k-images.idx3-ubyte";
}

struct MNISTImageLabel {
    MNISTImageLabel() = default;

    MNISTImageLabel(const std::vector<double>& img_values, const int label_int) {
        for (int y = 0; y < MNISTDataset::MNIST_IMAGE_SIZE; y++) {
            for (int x = 0; x < MNISTDataset::MNIST_IMAGE_SIZE; x++) {
                image_values(y, x) = img_values[y * MNISTDataset::MNIST_IMAGE_SIZE + x];
            }
        }
        label(label_int) = 1.0;
    }
    
    MatrixXd image_values = MatrixXd::Zero(MNISTDataset::MNIST_IMAGE_SIZE, MNISTDataset::MNIST_IMAGE_SIZE);
    VectorXd label = VectorXd::Zero(10);
};


namespace MNISTDataset {

    inline std::vector<std::vector<MNISTImageLabel>> batches;
    inline std::vector<MNISTImageLabel> test_examples;

    inline void load_data() {
        mnist_loader train_dataset( PATH_TO_TRAINING_IMAGES, PATH_TO_TRAINING_LABELS);
        mnist_loader test_dataset( PATH_TO_TESTING_IMAGES, PATH_TO_TESTING_LABELS);
        
        const int batches_count = train_dataset.size() / IMAGES_PER_BATCH;
        batches.resize(batches_count);

        const int test_examples_count = test_dataset.size();
        test_examples.resize(test_examples_count);

        for (int i = 0; i < test_examples_count; i++) {
            std::vector<double> image = test_dataset.images(i);
            int label = test_dataset.labels(i);

            test_examples[i] = MNISTImageLabel(image, label);
        }

        for (int i = 0; i < batches_count; i++) {
            batches[i].resize(IMAGES_PER_BATCH);
            for (int j = 0; j < IMAGES_PER_BATCH; j++) {
                int image_index = i * IMAGES_PER_BATCH + j;

                std::vector<double> image = train_dataset.images(image_index);
                int label = train_dataset.labels(image_index);

                batches[i][j] = MNISTImageLabel(image, label);
            }
        }
    }
}
#pragma once

#include <fstream>
#include <json.hpp>

#include <Eigen/Dense>
#include "mnist_loader.h"

#include "../logger/Logger.h"

using Eigen::VectorXd;

namespace MNISTDataset {
    constexpr int MNIST_IMAGE_SIZE = 28;
    constexpr int IMAGES_PER_BATCH = 20;
    
    inline std::string PATH_TO_TRAINING_LABELS = "D:\\My Projects\\AI Digit Recognizer\\src\\mnist_dataset\\train-labels.idx1-ubyte";
    inline std::string PATH_TO_TRAINING_IMAGES = "D:\\My Projects\\AI Digit Recognizer\\src\\mnist_dataset\\train-images.idx3-ubyte";
    
    inline std::string PATH_TO_TESTING_LABELS = "D:\\My Projects\\AI Digit Recognizer\\src\\mnist_dataset\\t10k-labels.idx1-ubyte";
    inline std::string PATH_TO_TESTING_IMAGES = "D:\\My Projects\\AI Digit Recognizer\\src\\mnist_dataset\\t10k-images.idx3-ubyte";
    
    inline std::string PATH_TO_FINE_TUNING = "D:\\My Projects\\AI Digit Recognizer\\src\\mnist_dataset\\fine_tuning_data.json";
}

struct MNISTImageLabel {
    MNISTImageLabel() = default;

    MNISTImageLabel(const std::vector<double>& img_values, const int label_int) {
        image_values = Eigen::VectorXd::Map(img_values.data(), 784);
        label(label_int) = 1.0;
    }
    
    VectorXd image_values = VectorXd::Zero(MNISTDataset::MNIST_IMAGE_SIZE * MNISTDataset::MNIST_IMAGE_SIZE);
    VectorXd label = VectorXd::Zero(10);
};


namespace MNISTDataset {

    inline std::vector<std::vector<MNISTImageLabel>> batches;
    inline std::vector<MNISTImageLabel> test_examples;

    inline void load_data() {
        mnist_loader train_dataset( PATH_TO_TRAINING_IMAGES, PATH_TO_TRAINING_LABELS );
        mnist_loader test_dataset( PATH_TO_TESTING_IMAGES, PATH_TO_TESTING_LABELS );

        std::ifstream json_file(PATH_TO_FINE_TUNING);
        nlohmann::json fine_tuning_data = nlohmann::json::parse(json_file);

        
        const int batches_count = train_dataset.size() / IMAGES_PER_BATCH;
        batches.resize(batches_count);
        
        const int test_examples_count = test_dataset.size();
        test_examples.resize(test_examples_count + fine_tuning_data.size());
        
        for (int i = 0; i < test_examples_count; i++) {
            std::vector<double> image = test_dataset.images(i);
            int label = test_dataset.labels(i);
            
            test_examples[i] = MNISTImageLabel(image, label);
        }
        for (int i = 0; i < fine_tuning_data.size(); i++) {
            std::vector<double> image = fine_tuning_data[i]["image"].get<std::vector<double>>();
            int label = static_cast<int>(fine_tuning_data[i]["label"].get<double>());
            
            test_examples[i + test_examples_count] = MNISTImageLabel(image, label);
        }   
        
        int curr_fine_tuned_index = 0;
        for (int i = 0; i < batches_count; i++) {
            batches[i].resize(IMAGES_PER_BATCH);
            for (int j = 0; j < IMAGES_PER_BATCH; j++) {
                int image_index = i * IMAGES_PER_BATCH + j;
                
                std::vector<double> image = {};
                int label = -1;
                
                if (j == IMAGES_PER_BATCH - 1) {
                    image = fine_tuning_data[curr_fine_tuned_index]["image"].get<std::vector<double>>();
                    label = static_cast<int>(fine_tuning_data[curr_fine_tuned_index]["label"].get<double>());
                    curr_fine_tuned_index = (curr_fine_tuned_index + 1) % fine_tuning_data.size();
                }
                else {
                    image = train_dataset.images(image_index);
                    label = train_dataset.labels(image_index);
                }
                
                batches[i][j] = MNISTImageLabel(image, label);
            }
        }
    }
    
    inline void print_image(MNISTImageLabel& img) {
        VectorXd& image = img.image_values;

        Logger::log("+----------------------------------------------------------+");
        for (int y = 0; y < MNIST_IMAGE_SIZE; y++) {
            std::string row = "| ";
            
            for (int x = 0; x < MNIST_IMAGE_SIZE; x++) {
                double val = image(y * MNIST_IMAGE_SIZE + x);
                if (val <= 0.0) row += "  ";
                else if (val <= 0.3) row += ". ";
                else if (val <= 0.7) row += "* ";
                else row += "# ";
            } 

            row += " |";
            Logger::log(row);
        }
        Logger::log("+----------------------------------------------------------+");
    }
}
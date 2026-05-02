#include "main.h"

using namespace godot;

void Main::_bind_methods() {
}

void Main::_ready() {
    godot_logger = GodotLogger();
    Logger::set_logger(&godot_logger);

    std::array<int, 4> neurons_per_layer = {
        MNISTDataset::MNIST_IMAGE_SIZE * MNISTDataset::MNIST_IMAGE_SIZE,
        16, 16,
        10
    };
    NeuralNetwork neural_net(neurons_per_layer);
    neural_net.save_to_json("D:\\My Projects\\AI Digit Recognizer\\src\\neural_network\\neural_network.json");
}

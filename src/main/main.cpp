#include "main.h"

using namespace godot;

void Main::_bind_methods() {
}

void Main::_ready() {
    godot_logger = GodotLogger();
    Logger::set_logger(&godot_logger);

    std::array<int, 3> neurons_per_layer = {3, 2, 3};
    NeuralNetwork neural_net(neurons_per_layer);

    std::ostringstream oss;
    oss << neural_net.get_layer(0).biases << "\n--------------------\n";
    oss << neural_net.get_layer(1).biases << "\n--------------------\n";
    oss << neural_net.get_layer(2).biases << "\n--------------------\n";
    oss << neural_net.get_weight_matrix(0).matrix << "\n--------------------\n";
    oss << neural_net.get_weight_matrix(1).matrix << "\n--------------------\n";
    Logger::log(oss.str());
}
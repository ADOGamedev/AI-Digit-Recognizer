#include "main.h"

using namespace godot;

void Main::_bind_methods() {
}

void Main::_ready() {
    godot_logger = GodotLogger();
    Logger::set_logger(&godot_logger);

    std::array<Layer, 3> layers = {
        Layer(3), Layer(2), Layer(3)
    };

    std::array<WeightMatrix, 2> weights = {
        WeightMatrix(2, 3), WeightMatrix(3, 2)
    };

    NeuralNetwork neural_net(layers, weights);


}

void main() {
    std::array<Layer, 3> layers = {
        Layer(3), Layer(2), Layer(3)
    };

    std::array<WeightMatrix, 2> weights = {
        WeightMatrix(2, 3), WeightMatrix(3, 2)
    };

    NeuralNetwork neural_net(layers, weights);
}
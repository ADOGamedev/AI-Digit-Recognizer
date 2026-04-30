#include "main.h"

using namespace godot;

void Main::_bind_methods() {
}

void Main::_ready() {
    godot_logger = GodotLogger();
    Logger::set_logger(&godot_logger);

    NeuralNetwork neural_net;
    neural_net.load_from_json("D:\\My Projects\\AI Digit Recognizer\\src\\neural_network\\neural_network.json");
    
    VectorXd inputs(3);
    inputs << 0.1, 0.2, 0.3;
    
    neural_net.set_input_layer(inputs);

    neural_net.compute_output();

    std::ostringstream oss2;
    oss2 << neural_net.get_output_layer().values << "\n--------------------\n";
    Logger::log(oss2.str());
}
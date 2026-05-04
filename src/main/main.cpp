#include "main.h"

using namespace godot;

void Main::_bind_methods() {
}

void Main::_ready() {
    godot_logger = GodotLogger();
    Logger::set_logger(&godot_logger);

    global = get_tree()->get_root()->get_node<Node>("Global");

    neural_net.load_from_json("D:\\My Projects\\AI Digit Recognizer\\src\\neural_network\\final_trained_network.json");

}

void Main::_process(double delta) {
    if (!global->get("update_requested")) {
        return;
    }

    VectorXd input(MNISTDataset::MNIST_IMAGE_SIZE * MNISTDataset::MNIST_IMAGE_SIZE);
    Array input_array = global->get("input_values");

    for (int i = 0; i < input_array.size(); i++) {
        input(i) = (double)input_array[i];
    }

    neural_net.set_input_layer(input);
    neural_net.compute_output();

    int guessed_digit;
    neural_net.get_output_layer().values.maxCoeff(&guessed_digit);

    global->set("guessed_digit", guessed_digit);
    global->set("update_requested", false);

    Logger::log(std::to_string((int)global->get("guessed_digit")));
}
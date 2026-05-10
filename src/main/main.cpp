#include "main.h"

using namespace godot;

int main() {
    Logger::Logger logger = Logger::Logger();
    Logger::set_logger(&logger);

    CNN cnn = CNN();
    cnn.load_from_json("G:\\My Projects\\AI Digit Recognizer\\src\\neural_network\\cnn\\cnn.json");

    MNISTDataset::load_data();

    Tensor input = {MNISTDataset::test_examples[0].image_values};

    cnn.set_input(input);
    cnn.do_forward_pass();

    for (int i = 0; i < cnn.layers_count; i++) {
        Logger::log("############################################");
        for (MatrixXd& img : cnn.get_img_tensor(i)) {
            print_image_matrix(img);
        }
    }
    Logger::log("############################################");
    for (MatrixXd& img : cnn.get_output_tensor()) {
        print_image_matrix(img);
    }
}



void Main::_bind_methods() {
}

void Main::_ready() {
    godot_logger = GodotLogger();
    Logger::set_logger(&godot_logger);

    global = get_tree()->get_root()->get_node_or_null("Global");

    neural_net.load_from_json("D:\\My Projects\\AI Digit Recognizer\\src\\neural_network\\training\\final_trained_network.json");

    if (!global) {
        Logger::log("Global node not found!");
        return;
    }
}

void Main::_process(double delta) {
    if (!global) {
        return;
    }
    
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

    Layer output_layer = neural_net.get_output_layer();
    output_layer.values.maxCoeff(&guessed_digit);

    global->set("guessed_digit", guessed_digit);

    Array godot_arr_outputs;
    for (int i = 0; i < 10; i++) {
        godot_arr_outputs.append(output_layer.values(i));
    }

    global->set("output_vector", godot_arr_outputs);
    global->set("update_requested", false);
}
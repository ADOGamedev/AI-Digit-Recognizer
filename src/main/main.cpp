#include "main.h"

using namespace godot;

int main() {
    Logger::Logger logger = Logger::Logger();
    Logger::set_logger(&logger);

    std::vector<int> kernels_per_layer = {2, 4};
    CNN cnn = CNN(kernels_per_layer);

    MNISTDataset::load_data();

    ConvolutionalLayer conv_layer(1, 1);

    Tensor tensor = {MNISTDataset::test_examples[3].image_values};

    conv_layer.set_image_tensor(tensor);
    
    Kernel h_kernel = Kernel(1, KERNEL_SIZE);
    MatrixXd h_kernel_values(KERNEL_SIZE, KERNEL_SIZE);
    h_kernel_values << -1,  0,  1,
                       -2,  0,  2,
                       -1,  0,  1;
    h_kernel.values = {h_kernel_values};

    // Bordes horizontales (gradiente vertical)
    Kernel v_kernel = Kernel(1, KERNEL_SIZE);
    MatrixXd v_kernel_values(KERNEL_SIZE, KERNEL_SIZE);
    v_kernel_values <<  1,  2,  1,
                        0,  0,  0,
                       -1, -2, -1;
    v_kernel.values = {v_kernel_values};

    // Todos los bordes (Laplaciano)
    Kernel l_kernel = Kernel(1, KERNEL_SIZE);
    MatrixXd l_kernel_values(KERNEL_SIZE, KERNEL_SIZE);
    l_kernel_values <<  0, -1,  0,
                       -1,  4, -1,
                        0, -1,  0;
    l_kernel.values = {l_kernel_values};

    // Bordes diagonales
    Kernel d_kernel = Kernel(1, KERNEL_SIZE);
    MatrixXd d_kernel_values(KERNEL_SIZE, KERNEL_SIZE);
    d_kernel_values << -1, -1,  0,
                       -1,  0,  1,
                        0,  1,  1;
    d_kernel.values = {d_kernel_values};

    conv_layer.kernels = {v_kernel};
    
    ConvolutionalLayer conv_layer2(1, 4);
    conv_layer2.image_tensor = conv_layer.do_convolution();
    conv_layer2.image_tensor = conv_layer2.do_max_pooling();
    
    conv_layer2.kernels = {h_kernel, v_kernel, l_kernel, d_kernel};

    print_image_matrix(tensor[0]);
    print_image_matrix(conv_layer.do_convolution()[0]);
    print_image_matrix(conv_layer2.do_convolution()[0]);
    print_image_matrix(conv_layer2.do_convolution()[1]);
    print_image_matrix(conv_layer2.do_convolution()[2]);
    print_image_matrix(conv_layer2.do_convolution()[3]);
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
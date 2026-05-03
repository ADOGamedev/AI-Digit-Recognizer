#include "main.h"

using namespace godot;

void Main::_bind_methods() {
}

void Main::_ready() {
    godot_logger = GodotLogger();
    Logger::set_logger(&godot_logger);

}


int main() {
    Logger::Logger logger = Logger::Logger();
    Logger::set_logger(&logger);

    MNISTDataset::load_data();

    NeuralNetwork neural_net;
    neural_net.load_from_json("D:\\My Projects\\AI Digit Recognizer\\src\\neural_network\\neural_network.json");

    Trainer trainer(&neural_net, 2.0);
    trainer.train(1000);
    trainer.set_learning_rate(0.5);
    trainer.train(2000);
    trainer.set_learning_rate(0.1);
    trainer.train(2000);

    neural_net.save_to_json("D:\\My Projects\\AI Digit Recognizer\\src\\neural_network\\second_attemp\\trained_network2.json");
}

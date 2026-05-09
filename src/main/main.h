#pragma once

#include <iostream>
#include <sstream>
#include <string>

#include <Eigen/Dense>
#include "mnist_loader.h"

#include <godot_cpp/classes/node.hpp>
#include <godot_cpp/classes/scene_tree.hpp>
#include <godot_cpp/classes/window.hpp>

#include "../logger/Logger.h"
#include "../neural_network/fully_connected/FCNeuralNetwork.h"
#include "../neural_network/cnn/CNN.h"
#include "../mnist_dataset/MNISTDataset.h"
#include "../trainer/Trainer.h"

class GodotLogger : public Logger::Logger {
    void log(const std::string& message) override {
        godot::UtilityFunctions::print(message.c_str());
    }
    void log(const char message) override {
        godot::UtilityFunctions::print(godot::String::chr(message));
    }
    void logerr(const std::string& message) override {
        godot::UtilityFunctions::printerr(message.c_str());
    }
};
namespace godot {

class Main : public Node {
    GDCLASS(Main, Node)

protected:
	static void _bind_methods();

    
public:
    Main() = default;
    ~Main() = default;

    void _ready() override;
    void _process(double delta) override;


private:
    GodotLogger godot_logger;

    FCNeuralNetwork neural_net{};
    Node* global;

};


}
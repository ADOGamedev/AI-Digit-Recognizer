#pragma once

#include <iostream>
#include <sstream>
#include <string>


#include <Eigen/Dense>

#include <godot_cpp/classes/node.hpp>

namespace godot {

class Main : public Node {
    GDCLASS(Main, Node)

protected:
	static void _bind_methods();

public:
    Main() = default;
    ~Main() = default;

    void _ready() override;

};


}
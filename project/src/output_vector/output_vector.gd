extends Control


var neuron_nodes : Array

func _ready() -> void:
	neuron_nodes = $neurons_vbox_cont.get_children()


func _process(_delta: float) -> void:
	for i in range(10):
		neuron_nodes[i].set_value(Global.output_vector[i])

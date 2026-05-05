extends Control

const TWEEN_DURATION = 0.4

@onready var digit_label := %digit_label
@onready var value_label := %value_label
@onready var neuron_progress_panel := %neuron_progress_panel

@export var digit : int = -1
var value = 0.0

var neuron_progress_mat : ShaderMaterial

func _ready() -> void:
	digit_label.text = str(digit)
	neuron_progress_mat = neuron_progress_panel.get_material()

func _process(_delta: float) -> void:
	value_label.text = str(value).pad_decimals(2)


func set_value(val: float) -> void:
	var tween = get_tree().create_tween().set_parallel(true)
	tween.tween_method(
		func(v): neuron_progress_mat.set_shader_parameter("value", v),
		neuron_progress_mat.get_shader_parameter("value"),
		value,
		TWEEN_DURATION
	)

	tween.tween_property(self, "value", val, TWEEN_DURATION)
	

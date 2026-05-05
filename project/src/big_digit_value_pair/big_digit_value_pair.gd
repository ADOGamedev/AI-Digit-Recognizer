extends Control

const TWEEN_DURATION = 0.4

@onready var digit_label := %digit_label
@onready var value_label := %value_label

var prev_value = -1.0
var value = 0.0

func _process(_delta: float) -> void:
	if prev_value != Global.output_vector[Global.guessed_digit]:
		change_value()

	prev_value = value

	digit_label.text = str(Global.guessed_digit)
	value_label.text = "Confidence:\n" + str(value).pad_decimals(2)


func change_value() -> void:
	var tween = get_tree().create_tween()
	tween.tween_property(self, "value", Global.output_vector[Global.guessed_digit], TWEEN_DURATION)


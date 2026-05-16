extends Node

var input_values : Array
var update_requested = false

var guessed_digit = -1
var probability = 0.0

var output_vector = [0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0]


func _input(event):
    if event is InputEventKey and event.pressed and event.keycode == KEY_F12:
        var img = get_viewport().get_texture().get_image()
        img.save_png("screenshot.png")
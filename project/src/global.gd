extends Node

var input_values : Array
var update_requested = false

var guessed_digit = -1
var probability = 0.0

var output_vector = [0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0]
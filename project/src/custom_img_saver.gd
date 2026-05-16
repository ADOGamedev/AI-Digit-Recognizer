extends Node

var FINE_TUNING_DATA_JSON_PATH = "D:\\My Projects\\AI Digit Recognizer\\src\\mnist_dataset\\fine_tuning_data.json"


func _input(event):
	var label = -1

	if event is InputEventKey and event.pressed:
		match event.keycode:
			KEY_0: label = 0
			KEY_1: label = 1
			KEY_2: label = 2
			KEY_3: label = 3
			KEY_4: label = 4
			KEY_5: label = 5
			KEY_6: label = 6
			KEY_7: label = 7
			KEY_8: label = 8
			KEY_9: label = 9
			_: return
	else:
		return

	add_image_label_pair(Global.input_values, label)


func add_image_label_pair(new_image: Array, new_label: int):
	var data = []

	if FileAccess.file_exists(FINE_TUNING_DATA_JSON_PATH):
		var file = FileAccess.open(FINE_TUNING_DATA_JSON_PATH, FileAccess.READ)
		var text = file.get_as_text()

		var parsed = JSON.parse_string(text)
		if parsed != null:
			data = parsed

	data.append({
		"image": new_image,
		"label": new_label
	})

	var json_text = JSON.stringify(data, "  ")

	var file = FileAccess.open(FINE_TUNING_DATA_JSON_PATH, FileAccess.WRITE)
	file.store_string(json_text)
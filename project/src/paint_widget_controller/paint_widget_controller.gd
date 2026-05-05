extends Control

@onready var paint_widget := %PaintWidget


func _on_reload_button_pressed() -> void:
	paint_widget.clear()


func _on_grid_button_pressed() -> void:
	paint_widget.toggle_grid_visibility()


func _on_pixelate_button_pressed() -> void:
	paint_widget.toggle_compressed_image_visibility()


func _on_invert_color_button_pressed() -> void:
	paint_widget.invert_color()

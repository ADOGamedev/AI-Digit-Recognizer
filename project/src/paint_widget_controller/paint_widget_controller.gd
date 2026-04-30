extends Control

@onready var paint_wiget := %PaintWidget


func _on_reload_button_pressed() -> void:
	paint_wiget.clear()


func _on_grid_button_pressed() -> void:
	paint_wiget.toggle_grid_visibility()


func _on_pixelate_button_pressed() -> void:
	paint_wiget.toggle_compressed_image_visibility()

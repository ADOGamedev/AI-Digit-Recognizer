extends Control

@onready var paint_wiget := %PaintWidget
@onready var paint_widget_margin := %paint_widget_margin


func _ready() -> void:
	var img_size = paint_wiget.get_image_node_size()
	paint_widget_margin.add_theme_constant_override("margin_left", (size.x - img_size.x) / 2.9)
	paint_widget_margin.add_theme_constant_override("margin_right", (size.x - img_size.x) / 2.9)
	paint_widget_margin.add_theme_constant_override("margin_top", (size.y - img_size.y) / 9)
	paint_widget_margin.add_theme_constant_override("margin_bottom", (size.y - img_size.y) / 3)

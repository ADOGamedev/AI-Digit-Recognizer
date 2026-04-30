extends Button

@export var icon_texture : Texture2D
@export var expand_margins = 5
@export var tween_duration = 0.05

@onready var icon_node = $icon_margin/icon
@onready var style_box_hover = get_theme_stylebox("hover")
@onready var style_box_normal = get_theme_stylebox("normal")
@onready var style_box_pressed = get_theme_stylebox("pressed")

func _ready() -> void:
	icon_node.texture = icon_texture

	style_box_hover.expand_margin_left = 0
	style_box_hover.expand_margin_right = 0
	style_box_hover.expand_margin_bottom = 0
	style_box_hover.expand_margin_top = 0

	style_box_normal.expand_margin_left = 0
	style_box_normal.expand_margin_right = 0
	style_box_normal.expand_margin_bottom = 0
	style_box_normal.expand_margin_top = 0

	style_box_pressed.expand_margin_left = 0
	style_box_pressed.expand_margin_right = 0
	style_box_pressed.expand_margin_bottom = 0
	style_box_pressed.expand_margin_top = 0


func _on_mouse_entered() -> void:
	var tween = get_tree().create_tween().set_trans(Tween.TRANS_SINE).set_ease(Tween.EASE_IN).set_parallel(true)
	tween.tween_property(style_box_hover, "expand_margin_left", expand_margins, tween_duration)
	tween.tween_property(style_box_hover, "expand_margin_right", expand_margins, tween_duration)
	tween.tween_property(style_box_hover, "expand_margin_bottom", expand_margins, tween_duration)
	tween.tween_property(style_box_hover, "expand_margin_top", expand_margins, tween_duration)

	style_box_normal.expand_margin_left = expand_margins
	style_box_normal.expand_margin_right = expand_margins
	style_box_normal.expand_margin_bottom = expand_margins
	style_box_normal.expand_margin_top = expand_margins

	style_box_pressed.expand_margin_left = expand_margins
	style_box_pressed.expand_margin_right = expand_margins
	style_box_pressed.expand_margin_bottom = expand_margins
	style_box_pressed.expand_margin_top = expand_margins


func _on_mouse_exited() -> void:
	var tween = get_tree().create_tween().set_trans(Tween.TRANS_SINE).set_ease(Tween.EASE_IN).set_parallel(true)
	tween.tween_property(style_box_normal, "expand_margin_left", 0, tween_duration)
	tween.tween_property(style_box_normal, "expand_margin_right", 0, tween_duration)
	tween.tween_property(style_box_normal, "expand_margin_bottom", 0, tween_duration)
	tween.tween_property(style_box_normal, "expand_margin_top", 0, tween_duration)

	style_box_hover.expand_margin_left = 0
	style_box_hover.expand_margin_right = 0
	style_box_hover.expand_margin_bottom = 0
	style_box_hover.expand_margin_top = 0

	style_box_pressed.expand_margin_left = 0
	style_box_pressed.expand_margin_right = 0
	style_box_pressed.expand_margin_bottom = 0
	style_box_pressed.expand_margin_top = 0


func _on_button_down() -> void:
	var tween = get_tree().create_tween().set_trans(Tween.TRANS_SINE).set_ease(Tween.EASE_IN).set_parallel(true)
	tween.tween_property(style_box_pressed, "expand_margin_left", 0, tween_duration)
	tween.tween_property(style_box_pressed, "expand_margin_right", 0, tween_duration)
	tween.tween_property(style_box_pressed, "expand_margin_bottom", 0, tween_duration)
	tween.tween_property(style_box_pressed, "expand_margin_top", 0, tween_duration)

	style_box_hover.expand_margin_left = 0
	style_box_hover.expand_margin_right = 0
	style_box_hover.expand_margin_bottom = 0
	style_box_hover.expand_margin_top = 0

func _on_button_up() -> void:
	var tween = get_tree().create_tween().set_trans(Tween.TRANS_SINE).set_ease(Tween.EASE_IN).set_parallel(true)
	tween.tween_property(style_box_hover, "expand_margin_left", expand_margins, tween_duration)
	tween.tween_property(style_box_hover, "expand_margin_right", expand_margins, tween_duration)
	tween.tween_property(style_box_hover, "expand_margin_bottom", expand_margins, tween_duration)
	tween.tween_property(style_box_hover, "expand_margin_top", expand_margins, tween_duration)

	style_box_normal.expand_margin_left = expand_margins
	style_box_normal.expand_margin_right = expand_margins
	style_box_normal.expand_margin_bottom = expand_margins
	style_box_normal.expand_margin_top = expand_margins

	style_box_pressed.expand_margin_left = expand_margins
	style_box_pressed.expand_margin_right = expand_margins
	style_box_pressed.expand_margin_bottom = expand_margins
	style_box_pressed.expand_margin_top = expand_margins
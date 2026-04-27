extends Control

@onready var image_node := $CenterContainer/image
@onready var compressed_image_node := $CenterContainer/compressed_image
@onready var overlay := $CenterContainer/overlay

@export var use_antialiasing := true

@export var BRUSH_SIZE_INCREMENT = 2

var time_since_update = 0.0
const UPDATE_INTERVAL = 1.0 / 24.0
var ANTIALISING_WIDTH = 1

const MNIST_DATASET_IMAGE_SIZE = 28
var compressed_image_array : Array

class ColorConfig:
	func _init(_bg_color, _brush_base_color, _brush_secondary_color) -> void:
		bg_color = _bg_color
		brush_base_color = _brush_base_color
		brush_secondary_color = _brush_secondary_color

	var bg_color := Color.BLACK
	var brush_base_color := Color.WHITE
	var brush_secondary_color := Color.BLACK

var DARK_COLOR_CONFIG = ColorConfig.new(Color.BLACK, Color.WHITE, Color.BLACK)

var curr_color_config = DARK_COLOR_CONFIG

var image: Image
var compressed_image : Image
@export var brush_size := 80
var brush_pixels = []

var prev_mouse_pos = null
var curr_color = Color.WHITE

@export var grid_visible = false


func _ready() -> void:
	init_compressed_image()
	_rebuild_canvas()


func _notification(what: int) -> void:
	if what == NOTIFICATION_RESIZED:
		_rebuild_canvas()


func _rebuild_canvas() -> void:
	var canvas_size = int(min(size.x, size.y))
	if canvas_size <= 0 or !overlay:
		return

	var new_size = 	Vector2(canvas_size, canvas_size)

	image_node.custom_minimum_size = new_size
	image_node.size = new_size

	compressed_image_node.custom_minimum_size = new_size
	compressed_image_node.size = new_size

	overlay.custom_minimum_size = new_size
	overlay.size = new_size

	image = Image.create(canvas_size, canvas_size, true, Image.FORMAT_RGBA8)
	image.fill(curr_color_config.bg_color)

	image_node.texture = ImageTexture.create_from_image(image)

	compute_brush_pixels()


func init_compressed_image() -> void:
	compressed_image = Image.create(MNIST_DATASET_IMAGE_SIZE, MNIST_DATASET_IMAGE_SIZE, true, Image.FORMAT_RGBA8)

	for i in range(MNIST_DATASET_IMAGE_SIZE):
		compressed_image_array.append([])
		compressed_image_array[i].resize(MNIST_DATASET_IMAGE_SIZE)
		compressed_image_array[i].fill(0)

	compressed_image_node.texture = ImageTexture.create_from_image(compressed_image)


func _process(delta: float) -> void:
	handle_input()

	overlay.queue_redraw()

	time_since_update += delta
	if time_since_update > UPDATE_INTERVAL:
		image_node.texture.update(image)
		update_compressed_image()
		time_since_update = 0.0


func handle_input() -> void:
	if Input.is_action_just_pressed("mouse_wheel_up"):
		brush_size = max(1, brush_size + BRUSH_SIZE_INCREMENT)
		compute_brush_pixels()
	elif Input.is_action_just_pressed("mouse_wheel_down"):
		brush_size = max(1, brush_size - BRUSH_SIZE_INCREMENT)
		compute_brush_pixels()

	if Input.is_mouse_button_pressed(MOUSE_BUTTON_LEFT):
		curr_color = curr_color_config.brush_base_color
	elif Input.is_mouse_button_pressed(MOUSE_BUTTON_RIGHT):
		curr_color = curr_color_config.brush_secondary_color
	else:
		prev_mouse_pos = null
		curr_color = curr_color_config.brush_base_color
		return

	var mouse_pos = image_node.get_local_mouse_position()
	var line_pos1 = mouse_pos
	var line_pos2 = prev_mouse_pos if prev_mouse_pos else mouse_pos

	line(line_pos1, line_pos2, curr_color)

	prev_mouse_pos = mouse_pos


func update_compressed_image() -> void:
	compressed_image = image.duplicate()
	compressed_image.resize(MNIST_DATASET_IMAGE_SIZE, MNIST_DATASET_IMAGE_SIZE, Image.INTERPOLATE_LANCZOS)
	compressed_image_node.texture.update(compressed_image)

	update_compressed_image_array()


func update_compressed_image_array() -> void:
	for y in range(MNIST_DATASET_IMAGE_SIZE):
		for x in range(MNIST_DATASET_IMAGE_SIZE):
			var value = compressed_image.get_pixel(x, y).r
			compressed_image_array[y][x] = value


func compute_brush_pixels() -> void:
	var radius = brush_size / 2
	var radius_antialising = radius + ANTIALISING_WIDTH if use_antialiasing else radius

	brush_pixels.clear()
	for x in range(-radius_antialising, radius_antialising + 1):
		for y in range(-radius_antialising, radius_antialising + 1):
			if Vector2(x, y).length() <= radius_antialising:
				brush_pixels.append(Vector2i(x, y))


func line(pos1: Vector2, pos2: Vector2, color: Color) -> void:
	if !pos1 or !pos2:
		return

	draw_capsule(pos1, pos2, color)


func paint(pos: Vector2, color: Color) -> void:
	var ipos = Vector2i(pos)
	var radius = brush_size / 2.

	for offset in brush_pixels:
		var pixel_pos = ipos + offset
		var dist = offset.length()

		draw_pixel(pixel_pos, radius, dist, color)


func draw_capsule(pos1: Vector2, pos2: Vector2, color: Color) -> void:
	var radius = brush_size / 2. + ANTIALISING_WIDTH

	var min_x = clamp(int(min(pos1.x, pos2.x) - radius), 0, image.get_width())
	var max_x = clamp(int(max(pos1.x, pos2.x) + radius), 0, image.get_width())

	var min_y = clamp(int(min(pos1.y, pos2.y) - radius), 0, image.get_height())
	var max_y = clamp(int(max(pos1.y, pos2.y) + radius), 0, image.get_height())

	var diff_vector = pos2 - pos1
	var diff_vector_length_squared = diff_vector.length_squared()


	for y in range(min_y, max_y + 1):
		for x in range(min_x, max_x + 1):
			var pixel_pos = Vector2(x, y)
			var p = pixel_pos - pos1

			var t = clamp( p.dot(diff_vector) / diff_vector_length_squared, 0.0, 1.0)
			var dist : float
			if diff_vector_length_squared < 0.0001:
				dist = pixel_pos.distance_to(pos1)
			else:
				dist = pixel_pos.distance_to(pos1 + t * diff_vector)

			if dist > radius:
				continue

			draw_pixel(pixel_pos, radius, dist, color)
			
# Radius: distance from source position to outer-most pixel
# Dist: distance from pixel to source position
func draw_pixel(pixel_pos: Vector2, radius: float, dist: float, color: Color) -> void:
	if pixel_out_of_bounds(pixel_pos):
		return

	var color_to_set = color
	var pixel_color = image.get_pixelv(pixel_pos)

	if use_antialiasing:
		var alpha = clamp(radius - dist, 0.0, 1.0)

		color_to_set = pixel_color.lerp(color, alpha * color.a)

	image.set_pixelv(pixel_pos, color_to_set)


func pixel_out_of_bounds(pos: Vector2) -> bool:
	return pos.x < 0 or pos.y < 0 or pos.x >= image.get_width() or pos.y >= image.get_height()


func clear() -> void:
	image.fill(curr_color_config.bg_color)
	image_node.texture.update(image)


func set_compressed_image_visibility(visible: bool) -> void:
	compressed_image_node.visible = visible
	overlay.queue_redraw()

func set_grid_visibility(visible: float) -> void:
	grid_visible = visible
	overlay.queue_redraw()

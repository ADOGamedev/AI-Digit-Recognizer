extends Control

@onready var image_node := %image
@onready var compressed_image_node := %compressed_image
@onready var grid := %grid
@onready var brush_overlay := %brush_overlay

@export var use_antialiasing := true

const UPDATE_INTERVAL = 1.0 / 20.0
var time_since_update = 0.0

const GLOBAL_UPDATE_INTERVAL = 1.0 / 1.8
const MAX_GLOBAL_UPDATE_INTERVAL = 1.2
var time_since_global_update = 0.0
var ANTIALISING_WIDTH = 1

const MNIST_DATASET_IMAGE_SIZE = 28
const MNIST_DATASET_IMAGE_MARGIN = 4
var centered_image_array : Array

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

var image : Image
var compressed_image : Image
var centered_image : Image
var brush_overlay_image : Image

@export var brush_size_ratio = 0.1
@onready var brush_size = image_node.size.y * brush_size_ratio
@export var BRUSH_SIZE_RATIO_INCREMENT = 0.01

var min_brush_size_ratio: float
var MAX_BRUSH_SIZE_RATIO = 0.3

var prev_mouse_pos = null
var curr_color = Color.WHITE

var _image_dirty := false
var _overlay_dirty := false
var _centered_image_dirty := false

func _ready() -> void:
	init_centered_image()
	init_compressed_image()
	_rebuild_canvas()

	min_brush_size_ratio = 1.0 / image_node.size.y
	brush_size = image_node.size.y * brush_size_ratio


func _notification(what: int) -> void:
	if what == NOTIFICATION_RESIZED:
		_rebuild_canvas()


func _rebuild_canvas() -> void:
	var canvas_size = int(min(size.x, size.y))
	if canvas_size <= 0 or !image_node:
		return

	var new_size = Vector2(canvas_size, canvas_size)

	image_node.custom_minimum_size = new_size
	image_node.size = new_size

	compressed_image_node.custom_minimum_size = new_size
	compressed_image_node.size = new_size

	brush_overlay.custom_minimum_size = new_size
	brush_overlay.size = new_size

	grid.custom_minimum_size = new_size
	grid.size = new_size

	image = Image.create(canvas_size, canvas_size, false, Image.FORMAT_RGBA8)
	image.fill(curr_color_config.bg_color)
	image_node.texture = ImageTexture.create_from_image(image)

	if !compressed_image:
		compressed_image = Image.create(MNIST_DATASET_IMAGE_SIZE, MNIST_DATASET_IMAGE_SIZE, false, Image.FORMAT_RGBA8)
		compressed_image_node.texture = ImageTexture.create_from_image(compressed_image)

	brush_overlay_image = Image.create(canvas_size, canvas_size, false, Image.FORMAT_RGBA8)
	brush_overlay.texture = ImageTexture.create_from_image(brush_overlay_image)

	min_brush_size_ratio = 1.0 / image_node.size.y
	brush_size = image_node.size.y * brush_size_ratio

	_overlay_dirty = true


func init_centered_image() -> void:
	centered_image = Image.create(MNIST_DATASET_IMAGE_SIZE, MNIST_DATASET_IMAGE_SIZE, false, Image.FORMAT_RGBA8)
	centered_image_array.resize(MNIST_DATASET_IMAGE_SIZE * MNIST_DATASET_IMAGE_SIZE)
	centered_image_array.fill(0.0)

func init_compressed_image() -> void:
	compressed_image = Image.create(MNIST_DATASET_IMAGE_SIZE, MNIST_DATASET_IMAGE_SIZE, false, Image.FORMAT_RGBA8)
	compressed_image_node.texture = ImageTexture.create_from_image(compressed_image)


func _process(delta: float) -> void:
	handle_input()

	if _overlay_dirty:
		_update_overlay()
		_overlay_dirty = false

	time_since_update += delta
	time_since_global_update += delta
	if time_since_update >= UPDATE_INTERVAL:
		time_since_update = 0.0
		if _image_dirty:
			if compressed_image_node.visible:
				update_compressed_image()
			else:
				image_node.texture.update(image)

			_image_dirty = false


	if time_since_global_update > GLOBAL_UPDATE_INTERVAL:
		if _centered_image_dirty or time_since_global_update > MAX_GLOBAL_UPDATE_INTERVAL:
			time_since_global_update = 0.0
			update_centered_image()
			update_global()

			_centered_image_dirty = false



func _input(event: InputEvent) -> void:
	if event is InputEventMouseMotion:
		_overlay_dirty = true

# Generated with AI
func _update_overlay() -> void:
	if !brush_overlay_image or !brush_overlay.texture:
		return

	brush_overlay_image.fill(Color.TRANSPARENT)

	var mouse_pos = image_node.get_local_mouse_position()
	var radius = brush_size / 2.0
	var outer = radius + 2.0

	var min_x = clamp(int(mouse_pos.x - outer - 1), 0, brush_overlay_image.get_width())
	var max_x = clamp(int(mouse_pos.x + outer + 1), 0, brush_overlay_image.get_width())
	var min_y = clamp(int(mouse_pos.y - outer - 1), 0, brush_overlay_image.get_height())
	var max_y = clamp(int(mouse_pos.y + outer + 1), 0, brush_overlay_image.get_height())

	for y in range(min_y, max_y + 1):
		for x in range(min_x, max_x + 1):
			var dist = Vector2(x, y).distance_to(mouse_pos)

			if dist > outer + 1.0 or pixel_out_of_bounds(Vector2(x, y)):
				continue

			var pixel_color = Color.TRANSPARENT

			if dist < radius - 1.0:
				pixel_color = Color(curr_color.r, curr_color.g, curr_color.b, 0.25)

			var white_alpha = clamp(1.0 - abs(dist - radius), 0.0, 1.0)
			pixel_color = pixel_color.lerp(Color.WHITE, white_alpha)

			var black_alpha = clamp(1.0 - abs(dist - radius - 1.5), 0.0, 1.0)
			pixel_color = pixel_color.lerp(Color.BLACK, black_alpha)

			brush_overlay_image.set_pixel(x, y, pixel_color)

	brush_overlay.texture.update(brush_overlay_image)


func handle_input() -> void:
	if Input.is_action_just_pressed("mouse_wheel_up"):
		update_brush_size_ratio(BRUSH_SIZE_RATIO_INCREMENT)
		_overlay_dirty = true
	elif Input.is_action_just_pressed("mouse_wheel_down"):
		update_brush_size_ratio(-BRUSH_SIZE_RATIO_INCREMENT)
		_overlay_dirty = true

	if Input.is_mouse_button_pressed(MOUSE_BUTTON_LEFT):
		curr_color = curr_color_config.brush_base_color
	elif Input.is_mouse_button_pressed(MOUSE_BUTTON_RIGHT):
		curr_color = curr_color_config.brush_secondary_color
	else:
		prev_mouse_pos = null
		curr_color = curr_color_config.brush_base_color
		return

	var mouse_pos = image_node.get_local_mouse_position()
	var line_pos2 = prev_mouse_pos if prev_mouse_pos else mouse_pos
	draw_capsule(mouse_pos, line_pos2, curr_color)
	prev_mouse_pos = mouse_pos

	_image_dirty = true
	_overlay_dirty = true
	_centered_image_dirty = true


func update_brush_size_ratio(increment: float) -> void:
	brush_size_ratio = min(MAX_BRUSH_SIZE_RATIO, max(min_brush_size_ratio, brush_size_ratio + increment))
	brush_size = image_node.size.y * brush_size_ratio


func update_compressed_image() -> void:
	compressed_image.copy_from(image)
	compressed_image.resize(MNIST_DATASET_IMAGE_SIZE, MNIST_DATASET_IMAGE_SIZE, Image.INTERPOLATE_LANCZOS)
	compressed_image_node.texture.update(compressed_image)


func update_centered_image_array() -> void:
	var raw: PackedByteArray = centered_image.get_data()
	for y in range(MNIST_DATASET_IMAGE_SIZE):
		for x in range(MNIST_DATASET_IMAGE_SIZE):
			var idx = (y * MNIST_DATASET_IMAGE_SIZE + x) * 4
			centered_image_array[y * MNIST_DATASET_IMAGE_SIZE + x] = raw[idx] / 255.0

# Minimal help of AI for Godot-specific stuff
func update_centered_image() -> void:
	centered_image.fill(Color.BLACK)
	
	var min_x = image.get_width()
	var max_x = 0
	var min_y = image.get_height()
	var max_y = 0

	var found = false
	var centroid = Vector2.ZERO

	var data = image.get_data()
	var image_width = image.get_width()
	var image_height = image.get_height()

	var weight_sum = 0

	for y in range(image_height):
		for x in range(image_width):
			var idx = (y * image_width + x) * 4
			var w = data[idx] / 255.0

			if w <= 0.1:
				continue

			weight_sum += w

			centroid.x += x * w
			centroid.y += y * w

			found = true

			if x < min_x: min_x = x
			if x > max_x: max_x = x
			if y < min_y: min_y = y
			if y > max_y: max_y = y

	centroid /= weight_sum

	if !found:
		update_centered_image_array()
		return

	var width = max_x - min_x + 1
	var height = max_y - min_y + 1

	var target_width = MNIST_DATASET_IMAGE_SIZE - 2 * MNIST_DATASET_IMAGE_MARGIN
	var target_height = MNIST_DATASET_IMAGE_SIZE - 2 * MNIST_DATASET_IMAGE_MARGIN

	if width <= 0 or height <= 0:
		update_centered_image_array()
		return	

	if width > height:
		target_height *= height / float(width)
	else:
		target_width *= width / float(height)

	var rect = Rect2i(min_x, min_y, width, height)
	var sub_image = image.get_region(rect)
	sub_image.resize(target_width, target_height, Image.INTERPOLATE_LANCZOS)

	centered_image.blit_rect(
		sub_image, Rect2i(0, 0, target_width, target_height),
		Vector2i((MNIST_DATASET_IMAGE_SIZE - target_width) / 2, (MNIST_DATASET_IMAGE_SIZE - target_height) / 2)
	)


	update_centered_image_array()

	

func update_global() -> void:
	Global.update_requested = true
	Global.input_values = centered_image_array

# Generated with help of AI. Though, I had implemented simillar stuff my self
func draw_capsule(pos1: Vector2, pos2: Vector2, color: Color) -> void:
	var radius = brush_size / 2.0 + ANTIALISING_WIDTH

	var min_x = clamp(int(min(pos1.x, pos2.x) - radius), 0, image.get_width())
	var max_x = clamp(int(max(pos1.x, pos2.x) + radius), 0, image.get_width())
	var min_y = clamp(int(min(pos1.y, pos2.y) - radius), 0, image.get_height())
	var max_y = clamp(int(max(pos1.y, pos2.y) + radius), 0, image.get_height())

	var diff_vector = pos2 - pos1
	var diff_vector_length_squared = diff_vector.length_squared()
	var use_point = diff_vector_length_squared < 0.0001

	for y in range(min_y, max_y + 1):
		for x in range(min_x, max_x + 1):
			var pixel_pos = Vector2(x, y)
			var dist: float
			if use_point:
				dist = pixel_pos.distance_to(pos1)
			else:
				var p = pixel_pos - pos1
				var t = clamp(p.dot(diff_vector) / diff_vector_length_squared, 0.0, 1.0)
				dist = pixel_pos.distance_to(pos1 + t * diff_vector)

			if dist > radius:
				continue

			draw_pixel(pixel_pos, radius, dist, color)


func draw_pixel(pixel_pos: Vector2, radius: float, dist: float, color: Color) -> void:
	if pixel_out_of_bounds(pixel_pos):
		return

	var color_to_set: Color
	if use_antialiasing:
		var alpha = clamp(radius - dist, 0.0, 1.0)
		color_to_set = image.get_pixelv(pixel_pos).lerp(color, alpha * color.a)
	else:
		color_to_set = color

	image.set_pixelv(pixel_pos, color_to_set)


func pixel_out_of_bounds(pos: Vector2) -> bool:
	return pos.x < 0 or pos.y < 0 or pos.x >= image.get_width() or pos.y >= image.get_height()


func clear() -> void:
	_rebuild_canvas()


func toggle_compressed_image_visibility() -> void:
	if compressed_image_node.visible:
		image_node.texture.update(image)
	else:
		update_compressed_image()

	compressed_image_node.visible = !compressed_image_node.visible

func toggle_grid_visibility() -> void:
	grid.visible = !grid.visible

func get_image_node_size() -> Vector2:
	return image_node.size

func invert_color() -> void:
	var curr = $CenterContainer.material.get_shader_parameter("invert")
	$CenterContainer.material.set_shader_parameter("invert", !curr)
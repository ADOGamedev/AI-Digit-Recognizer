extends TextureRect

@export var use_antialiasing := true

var time_since_update = 0.0
const UPDATE_INTERVAL = 1.0 / 30.0
var ANTIALISING_WIDTH = 1

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
@export var brush_size := 50
var brush_pixels = []

var prev_mouse_pos = null
var curr_color = Color.WHITE


func _ready() -> void:
	image = Image.create(size.x, size.y, false, Image.FORMAT_RGBA8)
	image.fill(curr_color_config.bg_color)

	texture = ImageTexture.create_from_image(image)

	compute_brush_pixels()


func _process(delta: float) -> void:
	handle_input()

	queue_redraw()
	
	print(Engine.get_frames_per_second())

	time_since_update += delta
	if time_since_update > UPDATE_INTERVAL:
		texture.update(image)
		time_since_update = 0.0


func handle_input() -> void:
	if Input.is_action_just_pressed("mouse_wheel_up"):
		brush_size = max(1, brush_size + 1)
		compute_brush_pixels()
	elif Input.is_action_just_pressed("mouse_wheel_down"):
		brush_size = max(1, brush_size - 1)
		compute_brush_pixels()

	if Input.is_mouse_button_pressed(MOUSE_BUTTON_LEFT):
		curr_color = curr_color_config.brush_base_color
	elif Input.is_mouse_button_pressed(MOUSE_BUTTON_RIGHT):
		curr_color = curr_color_config.brush_secondary_color
	else:
		prev_mouse_pos = null
		curr_color = curr_color_config.brush_base_color
		return

	var mouse_pos = get_local_mouse_position()
	var line_pos1 = mouse_pos
	var line_pos2 = prev_mouse_pos if prev_mouse_pos else mouse_pos

	line(line_pos1, line_pos2, curr_color)

	prev_mouse_pos = mouse_pos


func _draw() -> void:
	draw_brush_overlay()


func draw_brush_overlay() -> void:
	var mouse_pos = get_local_mouse_position()
	if pixel_out_of_bounds(mouse_pos):
		return

	const arc_width = 0.5
	var arc_radius = (brush_size - arc_width) / 2.
	var circle_radius = brush_size / 2.

	draw_circle(mouse_pos, circle_radius, curr_color, true)
	draw_arc(mouse_pos, arc_radius + 2*arc_width, 0, 2 * PI, 32, Color.BLACK, arc_width, true)
	draw_arc(mouse_pos, arc_radius, 0, 2 * PI, 32, Color.WHITE, arc_width, true)


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
	texture.update(image)

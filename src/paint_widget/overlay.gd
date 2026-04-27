extends Control

const MNIST_DATASET_IMAGE_SIZE = 28

func _draw() -> void:
	draw_brush_overlay()
	draw_grid_if_needed()


func draw_brush_overlay() -> void:
	var mouse_pos = get_local_mouse_position()
	if owner.pixel_out_of_bounds(mouse_pos):
		return

	const arc_width = 0.5
	var arc_radius = (owner.brush_size - arc_width) / 2.
	var circle_radius = owner.brush_size / 2.

	draw_circle(mouse_pos, circle_radius, owner.curr_color, true)
	draw_arc(mouse_pos, arc_radius + 2*arc_width, 0, 2 * PI, 32, Color.BLACK, arc_width, true)
	draw_arc(mouse_pos, arc_radius, 0, 2 * PI, 32, Color.WHITE, arc_width, true)


func draw_grid_if_needed() -> void:
	if !owner.grid_visible:
		return

	for i in range(1, MNIST_DATASET_IMAGE_SIZE):
		var pos = i * float(size.x) / MNIST_DATASET_IMAGE_SIZE
		var vertical_line_from = Vector2(pos, 0)
		var vertical_line_to = Vector2(pos, size.y)

		var horizontal_line_from = Vector2(0, pos)
		var horizontal_line_to = Vector2(size.x, pos)

		draw_line(vertical_line_from, vertical_line_to, Color.DIM_GRAY)
		draw_line(horizontal_line_from, horizontal_line_to, Color.DIM_GRAY)


func pixel_out_of_bounds(pos: Vector2) -> bool:
	return pos.x < 0 or pos.y < 0 or pos.x >= size.x or pos.y >= size.y

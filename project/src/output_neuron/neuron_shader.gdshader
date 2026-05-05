shader_type canvas_item;

uniform float value : hint_range(0.0, 1.0) = 0.5;
uniform float radius : hint_range(0.0, 0.5) = 0.2;

void fragment() {
    vec2 uv = UV;

    vec2 center = uv - vec2(0.5);
    vec2 abs_uv = abs(center);

    vec2 half_size = vec2(0.5 - radius);

    vec2 d = abs_uv - half_size;

    float outside = length(max(d, 0.0)) - radius;
    float rounded_rect = step(outside, 0.0); // 1 dentro, 0 fuera

    float mask = step(1. - uv.y, value);

    float alpha = rounded_rect * mask * 0.18;

    COLOR = vec4(vec3(1.0), alpha);
}
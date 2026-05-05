shader_type canvas_item;

uniform float radius : hint_range(0.0, 0.5) = 0.1;
uniform bool invert = false;

void fragment() {
    vec2 uv = UV - 0.5;
    vec2 size = vec2(1.0 / TEXTURE_PIXEL_SIZE) * TEXTURE_PIXEL_SIZE; // proporción real

    vec2 q = abs(uv) - vec2(0.5 - radius);
    float d = length(max(q, 0.0)) - radius;
    
    float alpha = 1.0 - smoothstep(-0.002, 0.002, d);
	COLOR = texture(TEXTURE, UV);
	
    COLOR.a *= alpha;
	
	if (invert) {
		COLOR = vec4(1. - vec3(COLOR.rgb), COLOR.a);
	}
}
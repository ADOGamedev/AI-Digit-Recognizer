// Made by Sleeth (Sleethcc on Github)
// I dont know if he used AI or not

shader_type canvas_item;

uniform vec4 color1 : source_color = vec4(0.2, 0.7, 1.0, 1.0);
uniform vec4 color2 : source_color = vec4(0.6, 0.9, 1.0, 1.0);
uniform vec4 color3 : source_color = vec4(0.1, 0.4, 0.9, 1.0);

uniform float speed = 0.1;
uniform float intensity = 0.25;
uniform float scale = 1.0;

float pattern(vec2 uv) {
    float t = TIME * speed;

    float a = sin((uv.x + t) * 6.28318);
    float b = sin((uv.y - t) * 6.28318);
    float c = sin((uv.x + uv.y + t * 0.5) * 6.28318);
    float d = cos((uv.x - uv.y - t * 0.7) * 6.28318);

    return (a + b + c + d) * 0.25;
}

void fragment() {
    vec2 uv = UV * scale;

    float p = pattern(uv);

    float mask1 = smoothstep(-0.5, 0.5, p);
    float mask2 = smoothstep(0.0, 1.0, sin(p * 3.1416));

    vec3 col = mix(color1.rgb, color2.rgb, mask1);
    col = mix(col, color3.rgb, mask2 * 0.5);

    col += p * intensity;

    COLOR = vec4(col, 1.0);
}

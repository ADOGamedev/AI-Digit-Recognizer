shader_type canvas_item;

uniform sampler2D screen_texture : hint_screen_texture, repeat_disable, filter_linear_mipmap;

uniform float refraction : hint_range(0.0, 0.03) = 0.012;
uniform float frost : hint_range(0.0, 6.0) = 2.5;
uniform float edge_width : hint_range(0.001, 0.12) = 0.055;
uniform float specular_power : hint_range(0.0, 3.0) = 1.8;
uniform float rim_strength : hint_range(0.0, 1.0) = 0.45;
uniform float transparency : hint_range(0.0, 1.0) = 0.82;
uniform vec4 tint : source_color = vec4(0.78, 0.92, 1.0, 1.0);
uniform float tint_strength : hint_range(0.0, 0.4) = 0.08;
uniform vec2 light_dir = vec2(0.45, -0.75);

void fragment() {
    vec4 shape = texture(TEXTURE, UV);

    if (shape.a < 0.01) {
        discard;
    } else {
        float s = edge_width;
        float al = texture(TEXTURE, UV - vec2(s, 0.0)).a;
        float ar = texture(TEXTURE, UV + vec2(s, 0.0)).a;
        float au = texture(TEXTURE, UV - vec2(0.0, s)).a;
        float ad = texture(TEXTURE, UV + vec2(0.0, s)).a;
        vec2 normal = vec2(al - ar, au - ad);
        float edge = length(normal);

        vec2 offset = normal * refraction;
        vec2 uv = SCREEN_UV + offset;

        float lod = frost * (1.0 - smoothstep(0.0, 0.6, edge));
        vec3 bg = textureLod(screen_texture, uv, lod).rgb;

        float ca = refraction * 0.35;
        bg.r = textureLod(screen_texture, uv + normal * ca, lod).r;
        bg.b = textureLod(screen_texture, uv - normal * ca, lod).b;

        bg = mix(bg, tint.rgb, tint_strength * shape.a);

        vec2 l = normalize(light_dir);
        vec2 n = normalize(normal + vec2(0.0001));
        float spec = dot(n, l);
        spec = pow(max(spec, 0.0), 5.0) * edge * specular_power;

        float rim = smoothstep(0.0, 0.45, edge) * rim_strength;
        float top_sheen = pow(1.0 - UV.y, 3.0) * 0.12 * shape.a;

        bg = bg + vec3(spec + rim + top_sheen);

        float final_alpha = shape.a * mix(transparency, 1.0, smoothstep(0.0, 0.5, edge));

        COLOR = vec4(clamp(bg, 0.0, 1.0), final_alpha);
    }
}
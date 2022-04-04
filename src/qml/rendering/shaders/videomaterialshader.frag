#version 440
layout(location = 0) in vec2 vTexCoord;
layout(location = 0) out vec4 fragColor;
layout(binding = 1) uniform sampler2D srcTex;

void main() {
    vec4 c = texture(srcTex, vTexCoord);

    fragColor = vec4(c.rgb, 1.0);
}

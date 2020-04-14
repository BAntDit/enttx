
#version 460
#extension GL_ARB_separate_shader_objects : enable

layout(location = 0) uniform sampler2D color;
layout(location = 1) uniform sampler2D alpha;

layout(location = 0) in vec2 vUv;
layout(location = 0) out vec4 outColor;

void main() {
    vec4 col = texture(color, vUv);
    float a = 1.0 - col.a;

    col.a = texture(alpha, vUv).r;

    // outColor = vec4(a * col.rgb / clamp(col.a, 0.001, 50000.0), a);
    outColor = vec4(col.rgb, 1.0);
}
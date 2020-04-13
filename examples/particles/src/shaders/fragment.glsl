#version 460
#extension GL_ARB_separate_shader_objects : enable

layout(location = 0) in vec2 vUv;

layout(location = 0) out vec4 outColor;

void main() {
    vec2 r = floor(vUv * 100);

    outColor = vec4(fract(sin(dot(r, vec2(12.9898,78.233))) * 43758.5453123), 0., 0., 1.);
}

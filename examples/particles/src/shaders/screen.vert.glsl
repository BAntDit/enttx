
#version 460
#extension GL_ARB_separate_shader_objects : enable

layout(location = 0) out vec2 vUv;

void main() {
    vUv = vec2((gl_VertexID << 1) & 2, gl_VertexID & 2);
    gl_Position = vec4(vUv * 2. - 1., 0., 1.);
}


#version 460
#extension GL_ARB_separate_shader_objects : enable

struct Transform
{
    vec4 xAxisTx;
    vec4 yAxisTy;
    vec4 zAxisTz;
};

layout(std430, binding = 0) readonly buffer xfrms
{
    Transform transforms[];
};

vec2 positions[6] = vec2[](
    vec2(-1.0, -1.0),
    vec2(-1.0, 1.0),
    vec2(1.0, 1.0),
    vec2(-1.0, -1.0),
    vec2(1.0, 1.0),
    vec2(1.0, -1.0)
);

layout(location = 1) uniform mat4 viewMatrix;
layout(location = 2) uniform mat4 projectionMatrix;

layout(location = 0) out vec2 vUv;

void main() {
    mat4 worldMatrix = transpose(mat4(
        transforms[gl_BaseInstance + gl_InstanceID].xAxisTx,
        transforms[gl_BaseInstance + gl_InstanceID].yAxisTy,
        transforms[gl_BaseInstance + gl_InstanceID].zAxisTz,
        vec4(0.0, 0.0, 0.0, 1.0)));

    mat4 matrixWorlView = viewMatrix * worldMatrix;

    vec4 viewPosition = matrixWorlView * vec4(0.0, 0.0, 0.0, 1.0);

    float scx = length(worldMatrix[0].xyz);
    float scy = length(worldMatrix[1].xyz);

    vec2 position = positions[gl_VertexID];

    vUv = position * 0.5 + 0.5;
    vUv.y = 1.0 - position.y;

    viewPosition.xy += vec2(position.x * scx, position.y * scy);

    gl_Position = projectionMatrix * viewPosition;
}

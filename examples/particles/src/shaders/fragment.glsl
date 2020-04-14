#version 460
#extension GL_ARB_separate_shader_objects : enable

layout(location = 0) in vec2 vUv;

layout(location = 0) out vec4 outColor;
layout(location = 1) out float outAlpha;

layout(location = 3) uniform vec3 ro;

float sdSeg(in vec3 pos, in float l, in float r) {
    pos.y -= min(l, max(0., pos.y));
    return length(pos) - r;
}

vec2 map(in vec3 pos) {
    return vec2(sdSeg(pos, 0.8, 0.05), 1.);
}

vec2 raymarch(in vec3 ro, in vec3 rd, in vec2 bounds) {
    float far = bounds.y;
    float near = bounds.x;
    float d = near;
    float m = 0.0;

    for (int i = 0; i < 64 && d < far; i++) {
        vec3 pos = ro + rd * d;

        vec2 res = map(pos);

        float h = res.x;

        m = res.y;

        if (abs(h) < near) break;

        d += h;
    }

    return vec2(d, m);
}

float weight(float z, float a) {
    return clamp(pow(min(1., a * 10.0) + 0.01, 3.0) * 1e8 * pow(1. - z * 0.9, 3.0), 1e-2, 3e3);
}

void main() {
    vec3 up = vec3(0.0, 1.0, 0.0);
    vec3 ww = -ro;
    vec3 uu = normalize(cross(ww, up));
    vec3 vv = normalize(cross(uu, ww));

    vec3 p = vec3(-1.0 + 2.0 * vUv, 1.0);

    vec3 rd = normalize(p.x * uu + p.y * vv + p.z * ww);

    float far = 6.;

    vec2 bounds = vec2(1e-3, far);

    vec2 res = raymarch(ro, rd, bounds);

    vec4 col = vec4(0.);

    if (res.x < far) {
        col = vec4(1., 0., 0., 1.);
    }

    col.rgb *= col.a;

    float w = weight(gl_FragCoord.z, col.a);

    outColor = vec4(col.rgb * w, col.a);
    outAlpha = col.a * w;
}

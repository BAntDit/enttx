#version 460
#extension GL_ARB_separate_shader_objects : enable

layout(location = 0) in vec2 vUv;

layout(location = 0) out vec4 outColor;
layout(location = 1) out float outAlpha;

layout(location = 3) uniform vec3 ro;
layout(location = 4) uniform float time;

const float pi = 3.1415926535;

float noise(in vec3 p) {
    vec3 i = floor(p);
    vec4 a = dot(i, vec3(1., 57., 21.)) + vec4(0., 57., 21., 78.);
    vec3 f = cos((p - i) * pi) * (-.5) + .5;

    a = mix(sin(cos(a) * a), sin(cos(1. + a) * (1. + a)), f.x);

    a.xy = mix(a.xz, a.yw, f.y);

    return mix(a.x, a.y, f.z);
}

#define OCTAVE_COUNT 5
float fbm(in vec3 p) {
    float n = 0.0;
    float amp = 0.5;

    for(int i = 0; i < OCTAVE_COUNT; i++) {
        n += noise(p) * amp;
        p *= 2.03;
        amp *= 0.5;
    }

    return n;
}

float sdShape(in vec3 p, in float r) {
    return length(p) - r;
}

float map(in vec3 p, in float r, in float amp) {
    float freq = 2.;

    return sdShape(p, r) + fbm(p * freq + amp * time) * amp;
}

vec4 explosion(in vec3 ro, in vec3 stp, in float exptime) {
    vec4 sum = vec4(0.0);
    vec3 pos = ro;

    float density = mix(0., 0.2, min(1., step(exptime, 0.25) + 1.0 / (2. * exptime)));
    float amp = mix(0.4 * .05, 0.4 * 1.2, clamp(exptime * 4., 0., 1.));
    float r = mix(0.6 * 0.5, mix(-0.5, 0.6, clamp(exptime * 8, 0., 1.0)), 1. / max(1., (1.1 * exptime - .3)));

    for(int i = 0; i < 64; i++) {
        float d = map(pos, r, amp);
        vec4 col = vec4(0.);

        if (d < 0.2)
        col = mix(vec4(vec3(2.), 1), vec4(1., 1., 0., 1.), d / 0.2);
        else if (d >= 0.2 && d < 0.4)
        col = mix(vec4(1, 1, 0, 1), vec4(.9, 0, 0, 0.5), (d - 0.2) / 0.2);
        else if (d >= 0.4 && d < 0.6)
        col = mix(vec4(1, 0, 0, 0.5), vec4(0, 0, 0, 0.4), (d - 0.4) / 0.2);
        // if (d >= 0.6 && d < 0.8)
        //     col = mix(vec4(0.2, 0.2, 0.2, 0.4), vec4(.5, .5, .5, 0.2), (d - 0.6) / 0.2);
        // else if (d >= 0.8 && d < 1.0)
        //     col = mix(vec4(0.5, .5, 0.5, .2), vec4(0, 0, 0, 0), (d - 0.8) / 0.2);

        col.a *= density;

        col.rgb *= col.a;

        sum = sum + col * (1.0 - sum.a);

        pos += stp;
    }

    return sum;
}

vec4 bomb(in vec3 ro, in vec3 rd) {
    float d = 0.1e-2;

    for (int i = 0; i < 64 && d < 10.; i++) {
        vec3 pos = ro + rd * d;

        float h = map(pos, 0.6, 0.);

        if (abs(h) < 0.1e-2) break;

        d += h;
    }

    vec4 col = vec4(0.);

    if (d < 10.) {
        vec3 pos = ro + rd * d;
        vec3 nor = normalize(pos);

        col.rgb = nor;
        col.a = 1.;
    }

    return col;
}

vec4 raymarch(in vec3 ro, in vec3 rd, in float stepSize) {
    float delay = 4.;
    return time < delay ? bomb(ro, rd) : explosion(ro, rd * stepSize, time - delay);
}

float weight(float z, float a) {
    return clamp(pow(min(1., a * 10.0) + 0.01, 3.0)  * 1e8 * pow(1. - z * 0.9, 3.0), 1e-2, 3e3);
}

void main() {
    vec2 uv = vUv * 2. - 0.5; // scale

    vec3 up = vec3(0.0, 1.0, 0.0);
    vec3 ww = -ro;
    vec3 uu = normalize(cross(ww, up));
    vec3 vv = normalize(cross(uu, ww));

    vec3 p = vec3(-1.0 + 2.0 * uv, 1.0);

    vec3 rd = normalize(p.x * uu + p.y * vv + p.z * ww);

    vec4 col = vec4(0.0);

    col.rgb *= col.a;

    float delta = 0.04;

    vec3 start = ro + rd * 5.; // make it closer to shape

    col = raymarch(start, rd, delta);

    float w = weight(gl_FragCoord.z, col.a);

    outColor = vec4(col.rgb * w, col.a);
    outAlpha = col.a * w;
}
